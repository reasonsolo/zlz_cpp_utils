//
// Created by lizhizhou on 2016/1/15.
//

#ifndef ZUTILS_TIMER_EVENT_H
#define ZUTILS_TIMER_EVENT_H

#include "common.h"
#include "atomic.h"
#include "time_utils.h"
#include "callbacks.h"

ZUTIL_NET_NAMESPACE_BEGIN
ZUTIL_NAMESPACE_USE;

NO_COPY_CLASS(TimerEvent) {
public:
    TimerEvent(TimerCallback&& cb, const int64_t when, const int32_t interval = 0);

    virtual ~TimerEvent();

    virtual void Run();

    bool isValid() const;

    int32_t interval() const {
        return interval_;
    }

    int64_t when() const {
        return when_;
    }

    uint64_t seq() const {
        return seq_;
    }

private:
    const TimerCallback& callback_;
    int64_t when_;
    int32_t interval_;
    uint64_t seq_;
    static AtomicU64 seq_counter_;
};

ZUTIL_NET_NAMESPACE_END

#endif //ZUTILS_TIMER_EVENT_H
