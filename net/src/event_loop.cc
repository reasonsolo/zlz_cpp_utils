//
// Created by zlz on 2016/1/14.
//

#include "event_loop.h"
#include "zthread.h"
#include "channel.h"
#include "poller.h"
#include "time_utils.h"

ZUTIL_NET_NAMESPACE_BEGIN

ZUTIL_NAMESPACE_USE;

__thread EventLoop* loop_in_thread = nullptr;

uint32_t EventLoop::poll_wait_time_ = 100000;

EventLoop::EventLoop() :
        poller_(Poller::GetDefaultPoller(this)),
        thread_id_(Thread::GetThreadId()),
        loop_count_(0),
        stop_(false),
        waiting_(false),
        current_channel_(nullptr),
        wake_up_channel_(nullptr),
        wake_up_sensor_() {
    assert(loop_in_thread == nullptr);
    loop_in_thread = this;
    wake_up_channel_ = new Channel(this, wake_up_sensor_.read_fd());
    wake_up_channel_->set_read_cb(std::bind(&GetWakingUpSignal, this));
    wake_up_channel_->EnableRead();
}

EventLoop::~EventLoop() {
    wake_up_channel_->DisableAll();
    wake_up_channel_->Remove();
    loop_in_thread = nullptr;
}

EventLoop* EventLoop::GetThreadLoop() {
    return loop_in_thread;
}

void EventLoop::Start() {
    DEBUG_LOG(ToString() << " starting");
    while (!stop_) {
        loop_count_++;
        active_channels_.clear();
        int64_t now = TimeUtils::GetTickMS();
        int64_t wait_time = GetNextLoopWaitTime() - now;
        next_wake_up_ = TimeUtils::GetTickMS() + wait_time;
        DEBUG_LOG(ToString() << " waiting time " << wait_time << " now " << now);
        if (wait_time > 0) {
            int32_t active_count = poller_->Poll(wait_time, active_channels_);
            DEBUG_LOG(ToString() << " poll due " << TimeUtils::GetTickMS()
                      << " get event num " << active_count);
            for (auto channel: active_channels_) {
                current_channel_ = channel;
                current_channel_->HandleEvent();
            }
        }
        current_channel_ = nullptr;
        HandleTimers();
    }
}

void EventLoop::Stop() {
    stop_ = true;
    WakeUp();
}

void EventLoop::WakeUp() {
    wake_up_sensor_.Notify();
}

void EventLoop::GetWakingUpSignal() {
    wake_up_sensor_.GetSignal(1);
}

void EventLoop::AddTimer(TimerEvent* event) {
    {
        ScopedWriteLock lock(&timer_queue_lock_);
        if (event->IsValid()) {
            DEBUG_LOG(ToString() << " inserting timer event " << event->Tostring());
            timer_queue_.insert(make_pair(event->when(), event));
        }
    }
    if (event->when() < next_wake_up_) {
        WakeUp();
    }
}

void EventLoop::RemoveTimer(TimerEvent* event) {
    ScopedWriteLock lock(&timer_queue_lock_);
    auto range_it = timer_queue_.equal_range(event->when());
    if (range_it.first == timer_queue_.end() && range_it.second == timer_queue_.end()) {
        DEBUG_LOG("cannot find any timer event in loop " << event->Tostring());
    } else {
        for (auto it = range_it.first; it != range_it.second; it++) {
            if (it->second == event) {
                DEBUG_LOG("get a matching event " << event->Tostring());
                timer_queue_.erase(it);
                break;
            }
        }
    }
}

int64_t EventLoop::GetNextLoopWaitTime() {
    {
        ScopedReadLock lock(&timer_queue_lock_);
        if (!timer_queue_.empty()) {
            return timer_queue_.begin()->first;
        }
    }
    return TimeUtils::GetTickMS() + poll_wait_time_;
}

void EventLoop::HandleTimers() {
    TimerQueue expired;
    TimerQueue::iterator end;
    vector<TimerEvent*> repeated;
    {
        ScopedReadLock lock(&timer_queue_lock_);
        int64_t now = TimeUtils::GetTickMS();
        end = timer_queue_.lower_bound(now);
        assert(end == timer_queue_.end() || now < end->first);
        for (auto it = timer_queue_.begin(); it != end; it++) {
            expired.insert(make_pair(it->first, it->second));
        }
    }
    for (auto it = expired.begin(); it != expired.end(); it++) {
        it->second->Run();
        if (it->second->interval() > 0) {
            repeated.push_back(it->second);
        }
    }
    if (!repeated.empty()) {
        ScopedWriteLock lock(&timer_queue_lock_);
        timer_queue_.erase(timer_queue_.begin(), end);
        for (auto item: repeated) {
            timer_queue_.insert(make_pair(item->when(), item));
        }
    }
}

void EventLoop::UpdateChannel(Channel* channel) {
    poller_->UpdateChannel(channel);
}

void EventLoop::RemoveChannel(Channel* channel) {
    poller_->RemoveChannel(channel);
}

void EventLoop::RunAfter(int64_t time, const EventCallBack& cb) {
    TimerEvent* event = new TimerEvent(cb, time);
    AddTimer(event);
}

ZUTIL_NET_NAMESPACE_END