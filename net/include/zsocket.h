//
// Created by lizhizhou on 2016/1/28.
//

#ifndef ZUTILS_SOCKET_H
#define ZUTILS_SOCKET_H

#include "../../utils/include/common.h"
#include "inet_address.h"

ZUTIL_NET_NAMESPACE_BEGIN

// struct tcp_info is in <netinet/tcp.h>
struct tcp_info;

class Socket {
public:
    explicit Socket(int32_t fd): fd_(fd) {
    }

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

    void SetTcpNoDelay(bool on = true);

    void SetKeepAlive(bool on = true);

    void SetNonBlock();

    string ToString() const;

private:

    int32_t fd_;

};

ZUTIL_NET_NAMESPACE_END

#endif //ZUTILS_SOCKET_H
