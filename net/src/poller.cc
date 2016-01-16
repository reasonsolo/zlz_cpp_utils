//
// Created by lizhizhou on 2016/1/15.
//

#include "poller.h"
#include "event_loop.h"
#include "channel.h"
#include "poll_poller.h"

ZUTIL_NET_NAMESPACE_BEGIN

Poller::Poller(EventLoop* loop): event_loop_(loop) {

}

Poller::~Poller() {

}

bool Poller::HasChannel(Channel* channel) {
    if (channel) {
        return channels_.find(channel->fd()) != channels_.end();
    }
    return false;
}

Poller* Poller::GetDefaultPoller(EventLoop * loop) {
    return new PollPoller(loop);
}



ZUTIL_NET_NAMESPACE_END