
//
// Created by zlz on 2016/1/14.
//

#include "event_loop.h"
#include "zthread.h"
#include "channel.h"
#include "poller.h"
#include "time_utils.h"
#include "log.h"

ZUTIL_NET_NAMESPACE_BEGIN

ZUTIL_NAMESPACE_USE;

__thread EventLoop* loop_in_thread = nullptr;

uint32_t EventLoop::poll_wait_time_ = 100000;

EventLoop::EventLoop() :
        poller_(Poller::GetDefaultPoller(this)),
        thread_id_(Thread::GetThreadId()),
        loop_count_(0),
        next_wake_up_(0),
        stop_(false),
        waiting_(false),
        state_(EventLoopState::kInit),
        current_channel_(nullptr),
        wake_up_channel_(nullptr),
        wake_up_sensor_() {
    assert(loop_in_thread == nullptr);
    loop_in_thread = this;
    wake_up_channel_ = new Channel(this, wake_up_sensor_.read_fd());
    wake_up_channel_->set_read_cb(std::bind(&GetWakingUpSignal, this));
    wake_up_channel_->EnableRead();
    pthread_rwlock_init(&timer_queue_lock_, nullptr);
    pthread_mutex_init(&pending_queue_lock_, nullptr);
}

EventLoop::~EventLoop() {
    wake_up_channel_->DisableAll();
    wake_up_channel_->Remove();
    loop_in_thread = nullptr;
}

EventLoop* EventLoop::GetThreadLoop() {
    if (!loop_in_thread) {
        loop_in_thread = new EventLoop();
    }
    return loop_in_thread;
}

void EventLoop::Start() {
    DEBUG_LOG(ToString() << " is starting");
    state_ = EventLoopState::kRunning;
    while (!stop_) {
        loop_count_++;
        active_channels_.clear();
        // wait time can be negative, use signed type here
        int64_t now = TimeUtils::GetTickMS();
        int64_t due_time = GetPollerDueTime(now);
        int64_t wait_time = static_cast<int64_t>(due_time) - now;
        next_wake_up_ = due_time;
        DEBUG_LOG(ToString() << " waiting time " << wait_time << " now " << now);
        if (wait_time > 0) {
            int32_t active_count = poller_->Poll(wait_time, active_channels_);
            DEBUG_LOG(ToString() << " poll due " << TimeUtils::GetTickMS()
                      << " get event num " << active_count
                      << " channels size " << active_channels_.size());
            for (auto& channel: active_channels_) {
                current_channel_ = channel;
                current_channel_->HandleEvent();
            }
        }
        if (wait_time < 0) {
            WARN_LOG(ToString() << " wait time is " << wait_time);
        }

        current_channel_ = nullptr;
        HandleTimers(due_time);
        HandlePendings();
    }
    state_ = EventLoopState::kStopped;
    DEBUG_LOG(ToString() << " is stopped");
}

void EventLoop::Stop() {
    DEBUG_LOG(ToString() << " is stopping");
    stop_ = true;
    WakeUp();
}

void EventLoop::WakeUp() {
    if (state_ == EventLoopState::kRunning) {
        DEBUG_LOG("wake up sensor");
        wake_up_sensor_.Notify();
    } else {
        DEBUG_LOG("event loop is not running, do not wake up");
    }
}

void EventLoop::GetWakingUpSignal() {
    DEBUG_LOG("wake up signal captured");
    wake_up_sensor_.GetSignal(1);
}

void EventLoop::AddTimer(TimerEvent* event) {
    if (event->IsValid()) {
        ScopedWriteLock lock(&timer_queue_lock_);
        DEBUG_LOG(ToString() << " inserting timer event " << event->ToString());
        timer_queue_.insert(make_pair(event->when(), event));
        if (event->when() < next_wake_up_ && !IsInLoop()) {
            WakeUp();
        }
    } else {
        WARN_LOG(ToString() << " finds " << event->ToString() << " is invalid, ignore this timer");
    }
}

void EventLoop::RemoveTimer(TimerEvent* event) {
    ScopedWriteLock lock(&timer_queue_lock_);
    auto range_it = timer_queue_.equal_range(event->when());
    if (range_it.first == timer_queue_.end() && range_it.second == timer_queue_.end()) {
        DEBUG_LOG("cannot find any timer event in loop " << event->ToString());
    } else {
        for (auto it = range_it.first; it != range_it.second; it++) {
            if (it->second == event) {
                DEBUG_LOG("get a matching event " << event->ToString());
                timer_queue_.erase(it);
                break;
            }
        }
    }
}

int64_t EventLoop::GetPollerDueTime(const int64_t now) {
    {
        ScopedReadLock lock(&timer_queue_lock_);
        if (!timer_queue_.empty()) {
            return timer_queue_.begin()->first;
        }
    }
    return now + poll_wait_time_;
}

void EventLoop::HandleTimers(const int64_t due_time) {
    DEBUG_LOG(ToString() << " handle timers " << due_time);
    TimerQueue::iterator end;
    vector<TimerEvent*> expired;
    vector<TimerEvent*> repeated;
    {
        ScopedReadLock lock(&timer_queue_lock_);
        end = timer_queue_.lower_bound(due_time + 1);
        assert(end == timer_queue_.end() || due_time <= end->first);
        for (auto it = timer_queue_.begin(); it != end; it++) {
            expired.push_back(it->second);
        }
    }
    DEBUG_LOG(ToString() << " get " << expired.size() << " timers");
    for (auto it = expired.begin(); it != expired.end(); it++) {
        DEBUG_LOG(ToString() << " due time " << due_time << " run timer " << (*it)->ToString());
        (*it)->Run();
        if ((*it)->interval() > 0) {
            repeated.push_back((*it));
        }
    }
    DEBUG_LOG(ToString() << " all expired timers are finished");
    {
        ScopedWriteLock lock(&timer_queue_lock_);
        timer_queue_.erase(timer_queue_.begin(), end);
        if (!repeated.empty()) {
            for (auto& item: repeated) {
                timer_queue_.insert(make_pair(item->when(), item));
            }
        }
    }
}

void EventLoop::UpdateChannel(Channel* channel) {
    poller_->UpdateChannel(channel);
}

void EventLoop::RemoveChannel(Channel* channel) {
    poller_->RemoveChannel(channel);
}

void EventLoop::RunAt(const EventCallBack& cb, int64_t time) {
    TimerEvent* event = new TimerEvent(cb, time);
    AddTimer(event);
}


void EventLoop::RunAfter(const EventCallBack& cb, int64_t time_delta) {
    RunAt(cb, TimeUtils::GetTickMS() + time_delta);
}

void EventLoop::RunEvery(const EventCallBack& cb, int64_t interval) {
    TimerEvent* event = new TimerEvent(cb, TimeUtils::GetTickMS(), interval);
    AddTimer(event);
}

void EventLoop::Run(const EventCallBack& cb) {
    {
        ScopedMutex lock(&pending_queue_lock_);
        pending_queue_.push_back(cb);
    }
    if (!IsInLoop()) {
        WakeUp();
    }
}

void EventLoop::HandlePendings() {
    PendingQueue copy_queue;
    {
        ScopedMutex lock(&pending_queue_lock_);
        std::swap(copy_queue, pending_queue_);
        assert(pending_queue_.empty());
    }
    for (auto& cb: copy_queue) {
        if (cb) {
            cb();
        }
    }
}

ZUTIL_NET_NAMESPACE_END