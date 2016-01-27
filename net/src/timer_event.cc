//
// Created by lizhizhou on 2016/1/15.
//

#include "timer_event.h"
#include "time_utils.h"
#include "log.h"

ZUTIL_NET_NAMESPACE_BEGIN

AtomicU64 TimerEvent::seq_counter_(0);

TimerEvent::TimerEvent(const TimerCallback& cb, const uint64_t when, const int32_t interval) :
        callback_(cb), when_(when), interval_(interval), seq_(seq_counter_.Incr()) {

}

TimerEvent::TimerEvent(TimerCallback&& cb, const uint64_t when, const int32_t interval) :
        callback_(std::move(cb)), when_(when), interval_(interval), seq_(seq_counter_.Incr()) {

}

TimerEvent::~TimerEvent() {

}

void TimerEvent::Run() {
    if (callback_) {
        DEBUG_LOG(ToString() << " run callback");
        callback_();
    } else {
        WARN_LOG(ToString() << " is an empty event");
    }

    if (interval_ > 0) {
        when_ += interval_;
    }
}

bool TimerEvent::IsValid() const {
    return when_ >= TimeUtils::GetTickMS();
}


ZUTIL_NET_NAMESPACE_END