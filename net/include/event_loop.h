//
// Created by zlz on 2016/1/14.
//

#ifndef ZUTILS_EVENT_LOOP_H
#define ZUTILS_EVENT_LOOP_H

#include "common.h"
#include "poller.h"
#include "timer_event.h"
#include "sensor.h"

ZUTIL_NET_NAMESPACE_BEGIN

NO_COPY_CLASS(EventLoop) {
typedef map<int64_t, TimerEvent*> TimerQueue;
public:
    EventLoop();

    virtual ~EventLoop();

    /*
     * loop forever
     */
    void Start();

    void Stop();

    void WakeUp();

    void AddTimer(TimerEvent* event);

    const pthread_t thread_id() const {
        return thread_id_;
    }

    uint64_t loop_count() const {
        return loop_count_;
    }

    int32_t GetNextLoopWaitTime();

private:

    void HandlePendings();

    Poller* poller_;
    const pthread_t thread_id_;

    uint64_t loop_count_;
    volatile bool stop_;

    Channel* current_channel_;
    vector<Channel*> active_channels_;

    Channel* wake_up_channel_;
    Sensor wake_up_sensor_;

    TimerQueue timer_queue_;
    list<BasicCallBack> pending_cbs_;

    static uint32_t pollWaitTime;
};

ZUTIL_NET_NAMESPACE_END

#endif //ZUTILS_EVENT_LOOP_H
