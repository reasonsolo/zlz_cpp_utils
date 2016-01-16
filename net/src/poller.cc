//
// Created by lizhizhou on 2016/1/15.
//

#include "poller.h"

ZUTIL_NET_NAMESPACE_BEGIN

Poller::Poller() {

}

Poller::~Poller() {

}

int32_t Poller::Poll(uint32_t time_ms, vector<Channel*>& active_pollables) {
    return 0;
}

void Poller::Init(uint32_t size) {

}

void Poller::Destroy() {

}

void Poller::SetEvent(Channel* event) {

}

Poller* Poller::GetDefaultPoller(EventLoop * loop) {
    return nullptr;
}
ZUTIL_NET_NAMESPACE_END