//
// Created by lizhizhou on 2016/1/15.
//
#include "net_utils.h"
#include <fcntl.h>

ZUTIL_NET_NAMESPACE_BEGIN

void set_fd_flags(int32_t fd, int32_t flag) {
    if (fcntl(fd, F_SETFL, fcntl(fd, F_GETFL, 0) | flag) == -1) {
        handle_sys_error("cannot set fd flag");
    }
}

bool has_fd_flags(int32_t fd, int32_t flag) {
    return (fcntl(fd, F_GETFL, 0) & flag) == flag;
}


ZUTIL_NET_NAMESPACE_END