//
// Created by zlz on 2016/1/16.
//


#include "poll_poller.h"
#include "channel.h"
#include "log.h"
#include <sys/poll.h>

ZUTIL_NET_NAMESPACE_BEGIN

PollPoller::PollPoller(EventLoop* loop): Poller(loop) {

}

PollPoller::~PollPoller() {

}

int32_t PollPoller::Poll(int64_t wait_time, vector<Channel*>& active_channels) {
    int32_t event_count = 0;
    if (fd_list_.size() > 0) {
        event_count = ::poll(&(*fd_list_.begin()), fd_list_.size(), wait_time);
        if (event_count > 0) {
            DEBUG_LOG(ToString() << " get " << event_count << " events");
            FillActiveChannel(event_count, active_channels);
        } else {
            DEBUG_LOG(ToString() << " nothing happend");
        }
    }
    return event_count;
}

void PollPoller::FillActiveChannel(int32_t event_count, vector<Channel*>& active_channels) {
    for (auto evfd: fd_list_) {
        if (event_count == 0) {
            break;
        }
        if (evfd.revents > 0) {
            event_count--;
            ChannelMap::iterator channel_it = channels_.find(evfd.fd);
            channel_it->second->set_revents(evfd.revents);
            active_channels.push_back(channel_it->second);
        }
    }
}

void PollPoller::UpdateChannel(Channel* channel) {
    if (channel->index() < 0) {
        // new channel, add new event
        struct pollfd pfd;
        pfd.fd = channel->fd();
        pfd.revents = 0;
        pfd.events = static_cast<short>(channel->events());
        channel->set_index(static_cast<int32_t>(fd_list_.size()));
        fd_list_.push_back(pfd);
        channels_[channel->fd()] = channel;
    } else {
        // update fd carefully
        assert(channels_.find(channel->fd()) != channels_.end());
        assert(channels_[channel->fd()] == channel);
        assert(fd_list_[channel->index()].fd == channel->fd());

        struct pollfd& pfd = fd_list_[channel->index()];
        pfd.events = static_cast<short>(channel->events());
        pfd.revents = 0;
        // TODO: deal with EV_NONE
    }

}

void PollPoller::RemoveChannel(Channel* channel) {
    if (channels_.find(channel->fd()) != channels_.end()) {
        // remove fd carefully
        assert(channels_[channel->fd()] == channel);
        assert(channel->index() >= 0 && channel->index() < static_cast<int32_t>(fd_list_.size()));

        ssize_t index = channel->index();
        if (static_cast<size_t>(index) != fd_list_.size() - 1) {
            // swap indices and references
            channels_[fd_list_.back().fd]->set_index(index);
            std::swap(fd_list_[index], fd_list_.back());
            // std::iter_swap(fd_list_.begin() + index, fd_list_.end() - 1);
        }
        channels_.erase(channel->fd());
        fd_list_.pop_back();
    }
}

ZUTIL_NET_NAMESPACE_END