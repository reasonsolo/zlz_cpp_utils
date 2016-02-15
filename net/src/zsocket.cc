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

Socket::Socket(const INetAddress& addr, bool nonblock) {
    fd_ = socket(addr.GetSockAddr()->sa_family, SOCK_STREAM, IPPROTO_TCP);
    if (fd_ < 0) {
        ERROR_LOG("cannot create socket");
    } else if (nonblock) {
        NetUtils::SetFdFlags(fd_, SOCK_NONBLOCK | SOCK_CLOEXEC);
    }
}

bool Socket::BindAddr(const INetAddress& addr) {
    if (bind(fd_, addr.GetSockAddr(), static_cast<socklen_t>(sizeof(struct sockaddr_in6))) < 0) {
        ERROR_LOG("cannot bind on addr " << addr.ip_port());
        return false;
    }
    return true;
}

bool Socket::Listen() {
    if (listen(fd_, SOMAXCONN) < 0) {
        ERROR_LOG(ToString() << "cannot listen on fd " << fd_);
        return false;
    }
    DEBUG_LOG(ToString() << " listen ok");
    return true;
}

int32_t Socket::Accept(INetAddress& peeraddr) {
    socklen_t addr_len = peeraddr.GetSockAddrLen();
    struct sockaddr_in6 addr;
    bzero(&addr, sizeof(addr));
    int32_t connfd = accept(fd_, static_cast<sockaddr*>(implict_cast<void*>(&addr)), &addr_len);
    NetUtils::SetFdFlags(connfd, SOCK_NONBLOCK | SOCK_CLOEXEC);
    peeraddr.set_addr6(addr);
    if (connfd < 0) {
        ERROR_LOG("cannot accept socket " << peeraddr.ip_port() << ", error " << errno);
    }
    return connfd;
}

void Socket::ShutDown() {
    shutdown(fd_, SHUT_RDWR);
}

void Socket::ShutDownWrite() {
    shutdown(fd_, SHUT_WR);
}

void Socket::ShutDownRead() {
    shutdown(fd_, SHUT_RD);
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

void Socket::SetReuseAddr(bool on) {
    int32_t optval = on;
    ::setsockopt(fd_, IPPROTO_TCP, SO_REUSEADDR,
                 &optval, static_cast<socklen_t>(sizeof optval));
}

int32_t Socket::GetError() {
    int32_t optval = 0;
    socklen_t len = static_cast<socklen_t>(sizeof(optval));
    if (getsockopt(fd_, SOL_SOCKET, SO_ERROR, &optval, &len) < 0) {
        return errno;
    }
    return optval;
}


ZUTIL_NET_NAMESPACE_END