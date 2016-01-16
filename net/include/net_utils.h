//
// Created by lizhizhou on 2016/1/15.
//

#ifndef ZUTILS_NET_UTILS_H
#define ZUTILS_NET_UTILS_H

#include "common.h"

ZUTIL_NET_NAMESPACE_BEGIN

void set_fd_flags(int32_t fd, int32_t flag);

void has_fd_flags(int32_t fd, int32_t flag);


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
