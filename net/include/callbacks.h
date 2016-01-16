//
// Created by lizhizhou on 2016/1/15.
//

#ifndef ZUTILS_CALLBACKS_H
#define ZUTILS_CALLBACKS_H


#include "common.h"
#include <functional>

ZUTIL_NET_NAMESPACE_BEGIN

typedef std::function<void()> BasicCallBack;
typedef std::function<void()> TimerCallback;


ZUTIL_NET_NAMESPACE_END

#endif //ZUTILS_CALLBACKS_H
