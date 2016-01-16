//
// Created by lizhizhou on 2016/1/15.
//
#include "net_utils.h"
#include "channel.h"
#include "event_loop.h"

ZUTIL_NET_NAMESPACE_BEGIN

Channel::Channel(EventLoop* loop, int32_t fd) :
        fd_(fd), events_(EV_NONE), revents_(EV_NONE), index_(-1), loop_(loop) {
    assert(loop_);
}

Channel::~Channel() {
    close(fd_);
}

void Channel::HandleEvent() {
    if ((revents_ & EV_NONE) && !(revents_ | EV_READ)) {
        if (close_cb_) {
            close_cb_();
        }
    }
    if (revents_ & EV_ERROR) {
        if (error_cb_) {
            error_cb_();
        }
    }
    if (revents_ & EV_READ) {
        if (read_cb_) {
            read_cb_();
        }
    }
    if (revents_ & EV_WRITE) {
        if (write_cb_) {
            write_cb_();
        }
    }
}

void Channel::Update() {
    loop_->UpdateChannel(this);
}

void Channel::Remove() {
    loop_->RemoveChannel(this);
}

void Channel::EnableRead() {
    events_ |= EV_READ;
    Update();
}

void Channel::EnableWrite() {
    events_ |= EV_WRITE;
    Update();
}

void Channel::DisableRead() {
    events_ &= ~EV_READ;
    Update();
}

void Channel::DisableWrite() {
    events_ &= ~EV_WRITE;
    Update();
}

void Channel::DisableAll() {
    events_ = EV_NONE;
    Update();
}


ZUTIL_NET_NAMESPACE_END