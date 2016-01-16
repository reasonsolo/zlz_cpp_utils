//
// Created by lizhizhou on 2016/1/15.
//

#ifndef ZUTILS_POLLER_H
#define ZUTILS_POLLER_H

#include "common.h"

ZUTIL_NET_NAMESPACE_BEGIN

class EventLoop;

class Channel;

class Poller {
public:
    Poller(EventLoop* loop);

    virtual ~Poller();

    virtual int32_t Poll(int64_t time_ms, vector<Channel*>& active_events) = 0;

    virtual void UpdateChannel(Channel* channel) = 0;

    virtual void RemoveChannel(Channel* channel) = 0;

    bool HasChannel(Channel* channel);

    static Poller* GetDefaultPoller(EventLoop* loop);

protected:
    typedef map<int32_t, Channel*> ChannelMap;
    EventLoop* event_loop_;
    ChannelMap channels_;

};


ZUTIL_NET_NAMESPACE_END

#endif //ZUTILS_POLLER_H
