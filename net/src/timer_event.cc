//
// Created by lizhizhou on 2016/1/15.
//

#include "timer_event.h"

ZUTIL_NET_NAMESPACE_BEGIN

Atomic64 TimerEvent::seq_counter_(0);

TimerEvent::TimerEvent(TimerCallback&& cb, const int64_t when, const int32_t interval) :
        callback_(std::move(cb)), when_(when), interval_(interval), seq_(seq_counter_.Incr()) {

}

TimerEvent::~TimerEvent() {

}

void TimerEvent::Run() {
    callback_();
    if (interval_ > 0) {
        when_ += interval_;
    }
}

bool TimerEvent::isValid() const {
    return when_ > TimeUtils::GetTimestampInMS();
}


ZUTIL_NET_NAMESPACE_END