//
// Created by lizhizhou on 2016/1/15.
//

#ifndef ZUTILS_POLLER_H
#define ZUTILS_POLLER_H

#include "common.h"
#include "event_loop.h"

ZUTIL_NET_NAMESPACE_BEGIN

class EventLoop;
class Channel;

NO_COPY_CLASS(Poller) {
public:
    Poller();

    virtual ~Poller();

    int32_t Poll(uint32_t time_ms, vector<Channel*>& active_events);

    void Init(uint32_t size);

    void Destroy();

    void SetEvent(Channel* event);

    static Poller* GetDefaultPoller(EventLoop* loop);

private:

    EventLoop* event_loop_;

};;


ZUTIL_NET_NAMESPACE_END

#endif //ZUTILS_POLLER_H
