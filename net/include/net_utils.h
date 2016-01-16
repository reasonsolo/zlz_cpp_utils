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

void set_fd_flags(int32_t fd, int32_t flag);

bool has_fd_flags(int32_t fd, int32_t flag);


static class IgnoreSigPipe
{
public:
    IgnoreSigPipe()
    {
        ::signal(SIGPIPE, SIG_IGN);
    }
} _never_use_this;

ZUTIL_NET_NAMESPACE_END

#endif //ZUTILS_UTILS_H
