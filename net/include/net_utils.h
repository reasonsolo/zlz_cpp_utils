//
// Created by lizhizhou on 2016/1/15.
//

#ifndef ZUTILS_NET_UTILS_H
#define ZUTILS_NET_UTILS_H

#include "common.h"

ZUTIL_NET_NAMESPACE_BEGIN

#define EV_NONE  0
#define EV_READ  POLLIN | POLLPRI
#define EV_WRITE POLLOUT
#define EV_RDWR  POLLIN | POLLPRI | POLLOUT
#define EV_CLOSE POLLHUP
#define EV_ERROR POLLERR


class NetUtils {
public:

    static void SetFdFlags(int32_t fd, int32_t flag);

    static bool HasFdFlags(int32_t fd, int32_t flag);

    /*
     * implement these on demand
     */
    static void SockAddr2IP(const struct sockaddr& addr, string& ip);

    static void SockAddr2IPPort(const struct sockaddr& addr, string& ip, int32_t& port);

    static void SockAddr2IPPort(const struct sockaddr& addr, string& ip_port);

};


static class IgnoreSigPipe {
public:
    IgnoreSigPipe() {
        ::signal(SIGPIPE, SIG_IGN);
    }
} _never_use_this;

ZUTIL_NET_NAMESPACE_END

#endif //ZUTILS_UTILS_H
