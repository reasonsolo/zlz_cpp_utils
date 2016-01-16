//
// Created by lizhizhou on 2016/1/15.
//

#include "pollable.h"
#include "net_utils.h"
#include <fcntl.h>
#include <net/if.h>
#include <sys/un.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <signal.h>

ZUTIL_NET_NAMESPACE_BEGIN

Channel::Channel() : fd_(-1), events_(-1) {

}

Channel::~Channel() {
    Close();
}

void Channel::Detach() {
    fd_ = -1;
}

void Channel::Close() {
    if (fd_ > 0) {
        close(fd_);
    }
}

void Channel::CloseRead() {
    if (fd_ > 0) {
        shutdown(fd_, SHUT_RD);
    }
}

void Channel::CloseWrite() {
    if (fd_ > 0) {
        shutdown(fd_, SHUT_WR);
    }
}

void Channel::CloseBoth() {
    if (fd_ > 0) {
        shutdown(fd_, SHUT_RDWR);
    }
}

void Channel::SetNonBlock() {
    set_fd_flags(fd_, O_NONBLOCK);
}

void Channel::HandleEvent() {

}
ZUTIL_NET_NAMESPACE_END