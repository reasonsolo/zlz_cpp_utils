//
// Created by zlz on 2016/1/14.
//

#include "event_loop.h"
#include "zthread.h"

ZUTIL_NET_NAMESPACE_BEGIN

ZUTIL_NAMESPACE_USE;

class Poller;

uint32_t EventLoop::pollWaitTime = 100000;

EventLoop::EventLoop() :
        poller_(Poller::GetDefaultPoller(this)),
        thread_id_(Thread::GetThreadId()),
        loop_count_(0),
        stop_(false),
        current_channel_(nullptr),
        wake_up_channel_(nullptr) {
    wake_up_channel_ = new Channel;
    wake_up_channel_
}

EventLoop::~EventLoop() {

}

void EventLoop::Start() {
    while (!stop_) {
        loop_count_++;
        active_channels_.clear();
        int32_t active_count = poller_->Poll(GetNextLoopWaitTime(), active_channels_);
        for (auto channel: active_channels_) {
            current_channel_ = channel;
            current_channel_->HandleEvent();
        }
        current_channel_ = nullptr;
        HandlePendings();

    }
}

void EventLoop::Stop() {
    stop_ = true;
}


void EventLoop::WakeUp() {
    wake_up_sensor_.Notify();
}

void EventLoop::AddTimer(TimerEvent* event) {

}

int32_t EventLoop::GetNextLoopWaitTime() {
    return pollWaitTime;
}

void EventLoop::HandlePendings() {

}

ZUTIL_NET_NAMESPACE_END