//
// Created by zlz on 2016/1/28.
//

#include "zsocket.h"
#include <sys/socket.h>
#include <netinet/tcp.h>
#include <netinet/in.h>
#include "log.h"
#include "net_utils.h"

ZUTIL_NET_NAMESPACE_BEGIN

bool Socket::BindAddr(const INetAddress& addr) {
    if (bind(fd_, addr.GetSockAddr(), static_cast<socklen_t>(sizeof(struct sockaddr_in6))) < 0) {
        ERROR_LOG("cannot bind on addr " << addr.ip_port());
        return false;
    }
    return true;
}

bool Socket::Listen() {
    if (listen(fd_, SOMAXCONN) < 0) {
        ERROR_LOG("cannot listen on fd " << fd_);
        return false;
    }
    return true;
}

int32_t Socket::Accept(INetAddress& peeraddr) {
    socklen_t addr_len = peeraddr.GetSockAddrLen();
    int32_t connfd = accept(fd_, peeraddr.GetSockAddr(), &addr_len);
    NetUtils::SetFdFlags(connfd, SOCK_NONBLOCK | SOCK_CLOEXEC);
    if (connfd < 0) {
        ERROR_LOG("cannot accept socket " << peeraddr.ip_port() << ", error " << errno);
    }
    return connfd;
}

void Socket::SetKeepAlive(bool on) {
    int32_t optval = on;
    ::setsockopt(fd_, IPPROTO_TCP, SO_KEEPALIVE,
                 &optval, static_cast<socklen_t>(sizeof optval));
}

void Socket::SetTcpNoDelay(bool on) {
    int32_t optval = on;
    ::setsockopt(fd_, IPPROTO_TCP, TCP_NODELAY,
                 &optval, static_cast<socklen_t>(sizeof optval));
}

void Socket::SetNonBlock() {
    NetUtils::SetFdFlags(fd_, SOCK_NONBLOCK | SOCK_CLOEXEC);
}


ZUTIL_NET_NAMESPACE_END