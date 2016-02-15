//
// Created by lizhizhou on 2016/1/28.
//

#ifndef ZUTILS_SOCKET_H
#define ZUTILS_SOCKET_H

#include "common.h"
#include "inet_address.h"

ZUTIL_NET_NAMESPACE_BEGIN

// struct tcp_info is in <netinet/tcp.h>
struct tcp_info;

class Socket {
public:
    Socket(int32_t fd) : fd_(fd) {
    }

    explicit Socket(const INetAddress& addr, bool nonblock);

    ~Socket() {
        if (fd_ >= 0) {
            close(fd_);
        }
    }

    int32_t fd() const {
        return fd_;
    }

    bool BindAddr(const INetAddress& addr);

    bool Listen();

    int32_t Accept(INetAddress& peeraddr);

    void ShutDown();

    void ShutDownRead();

    void ShutDownWrite();

    void SetTcpNoDelay(bool on = true);

    void SetKeepAlive(bool on = true);

    void SetReuseAddr(bool on = true);

    void SetNonBlock();

    int32_t GetError();

    string ToString() const {
        return StringUtils::ToString("socket-", fd_);
    }

private:

    int32_t fd_;

};

ZUTIL_NET_NAMESPACE_END

#endif //ZUTILS_SOCKET_H
