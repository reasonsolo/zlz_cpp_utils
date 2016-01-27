//
// Created by zlz on 2016/1/14.
//

#ifndef ZUTILS_EVENT_LOOP_H
#define ZUTILS_EVENT_LOOP_H

#include "common.h"
#include "timer_event.h"
#include "sensor.h"

ZUTIL_NET_NAMESPACE_BEGIN

class Poller;
class Channel;

enum class EventLoopState {
    kInit,
    kRunning,
    kStopped
};

NO_COPY_CLASS(EventLoop) {
    typedef std::multimap<int64_t, TimerEvent*> TimerQueue;
public:
    EventLoop();

    virtual ~EventLoop();

    /*
     * loop forever
     */
    static EventLoop* GetThreadLoop();

    void Start();

    void Stop();

    void WakeUp();

    void RunAfter(int64_t time, const EventCallBack& cb);

    void RunEvery(int64_t interval, const EventCallBack& cb);

    void AddTimer(TimerEvent* event);

    void RemoveTimer(TimerEvent* event);

    void UpdateChannel(Channel* channel);

    void RemoveChannel(Channel* channel);

    const pthread_t thread_id() const {
        return thread_id_;
    }

    uint64_t loop_count() const {
        return loop_count_;
    }


    string ToString() const {
        return StringUtils::ToString("EventLoop@", this, ":", thread_id_);
    }

private:

    int64_t GetPollerDueTime(const int64_t now);

    void GetWakingUpSignal();

    void HandlePendings();

    void HandleTimers(const int64_t due_time);

    Poller* poller_;
    const pthread_t thread_id_;

    uint64_t loop_count_;
    uint64_t next_wake_up_;
    volatile bool stop_;
    volatile bool waiting_;
    EventLoopState state_;

    Channel* current_channel_;
    // TODO: use shared_ptr
    vector<Channel*> active_channels_;

    int32_t wake_up_fd_;
    Channel* wake_up_channel_;
    Sensor wake_up_sensor_;

    pthread_rwlock_t timer_queue_lock_;
    TimerQueue timer_queue_;

    static uint32_t poll_wait_time_;
};

ZUTIL_NET_NAMESPACE_END

#endif //ZUTILS_EVENT_LOOP_H
