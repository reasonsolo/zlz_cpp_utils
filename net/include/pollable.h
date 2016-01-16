
//
// Created by lizhizhou on 2016/1/15.
//

#ifndef ZUTILS_POLLABLE_H
#define ZUTILS_POLLABLE_H

#include "common.h"
#include <sys/timerfd.h>

ZUTIL_NET_NAMESPACE_BEGIN

/*
 * poller should behave as following codes
 */
enum class PollEvent {
    kNone = 0,   // do nothing
    kRead,
    kWrite,
    kReadWrte,
    kClose,
    kRemove,
    kDestroy,
    kRelease
};

class EventLoop;

class Channel {
public:
    Channel();
    virtual ~Channel();

    int32_t fd() const {
        return fd_;
    }

    int32_t events() const {
        return events_;
    }

    void Detach();

    virtual void Close();

    void CloseRead();

    void CloseWrite();

    void CloseBoth();

    void SetNonBlock();

    virtual void HandleEvent();

private:

    int32_t fd_;
    int32_t events_;
    EventLoop* loop_;
};

ZUTIL_NET_NAMESPACE_END


#endif //ZUTILS_POLLABLE_H
