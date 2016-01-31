//
// Created by lizhizhou on 2016/1/15.
//

#ifndef ZUTILS_POLLABLE_H
#define ZUTILS_POLLABLE_H

#include "common.h"
#include "net_utils.h"
#include "callbacks.h"
#include "event_loop.h"
#include "string_utils.h"
#include <sys/poll.h>

ZUTIL_NET_NAMESPACE_BEGIN


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

    bool IsWritable() const {
        return implict_cast<bool>((revents_) & (EV_WRITE));
    }

    bool IsReadable() const {
        return implict_cast<bool>((revents_) & (EV_READ));
    }

    void set_read_cb(const EventCallBack& cb) {
        read_cb_ = std::move(cb);
    }

    void set_write_cb(const EventCallBack& cb) {
        write_cb_ = std::move(cb);
    }

    void set_close_cb(const EventCallBack& cb) {
        close_cb_ = std::move(cb);
    }

    void set_error_cb(const EventCallBack& cb) {
        error_cb_ = std::move(cb);
    }

    string ToString() const {
        return StringUtils::ToString("Channel@", fd_);
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

typedef std::shared_ptr<Channel> ChannelPtr;

ZUTIL_NET_NAMESPACE_END


#endif //ZUTILS_POLLABLE_H
