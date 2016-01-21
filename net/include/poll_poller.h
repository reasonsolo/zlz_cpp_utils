//
// Created by zlz on 2016/1/16.
//

#ifndef ZUTILS_POLL_POLLER_H
#define ZUTILS_POLL_POLLER_H

#include "poller.h"
#include "string_utils.h"
#include <sys/poll.h>

ZUTIL_NET_NAMESPACE_BEGIN

class PollPoller : public Poller {
    typedef std::vector<struct pollfd> PollFdList;
public:
    PollPoller(EventLoop* loop);

    virtual ~PollPoller();

    virtual int32_t Poll(int64_t time_ms, vector<Channel*>& active_channels);

    void UpdateChannel(Channel* event);

    void RemoveChannel(Channel* event);

    string ToString() const {
        return StringUtils::ToString("poller@", this);
    }

private:
    void FillActiveChannel(int32_t event_count, vector<Channel*>& channels);

    PollFdList fd_list_;

};

ZUTIL_NAMESPACE_USE;

ZUTIL_NET_NAMESPACE_END

#endif //ZUTILS_POLL_POLLER_H


