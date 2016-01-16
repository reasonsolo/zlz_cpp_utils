//
// Created by lizhizhou on 2016/1/15.
//

#ifndef ZUTILS_POLLABLE_H
#define ZUTILS_POLLABLE_H

#include "common.h"
#include "net_utils.h"
#include "callbacks.h"
#include <sys/poll.h>

ZUTIL_NET_NAMESPACE_BEGIN

class EventLoop;

class Channel {
public:
    Channel(EventLoop* loop, int32_t fd);

    virtual ~Channel();

    int32_t fd() const {
        return fd_;
    }

    int32_t events() const {
        return events_;
    }

    int32_t index() const {
        return index_;
    }

    void set_revents(int32_t revents) {
        revents_ = revents;
    }

    void set_index(int32_t index) {
        index_ = index;
    }

    void EnableRead();

    void EnableWrite();

    void DisableRead();

    void DisableWrite();

    void DisableAll();

    void Remove();

    virtual void HandleEvent();

    inline void set_read_cb(EventCallBack&& cb) {
        read_cb_ = std::move(cb);
    }

    inline void set_write_cb(EventCallBack&& cb) {
        write_cb_ = std::move(cb);
    }

    inline void set_close_cb(EventCallBack&& cb) {
        close_cb_ = std::move(cb);
    }

    inline void set_error_cb(EventCallBack&& cb) {
        error_cb_ = std::move(cb);
    }

private:

    inline void Update();

    int32_t fd_;
    int32_t events_;
    int32_t revents_;
    int32_t index_;
    EventLoop* loop_;

    EventCallBack read_cb_;
    EventCallBack write_cb_;
    EventCallBack close_cb_;
    EventCallBack error_cb_;
};

ZUTIL_NET_NAMESPACE_END


#endif //ZUTILS_POLLABLE_H
