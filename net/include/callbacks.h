//
// Created by lizhizhou on 2016/1/15.
//

#ifndef ZUTILS_CALLBACKS_H
#define ZUTILS_CALLBACKS_H


#include "common.h"
#include <functional>

ZUTIL_NET_NAMESPACE_BEGIN

typedef std::function<void()> EventCallBack;
typedef std::function<void()> TimerCallback;
typedef std::function<void()> ReadEventCallback;
typedef std::function<void()> WriteEventCallback;
typedef std::function<void()> CloseEventCallback;
typedef std::function<void()> ConnectEventCallback;
typedef std::function<void()> ErrorEventCallback;


ZUTIL_NET_NAMESPACE_END

#endif //ZUTILS_CALLBACKS_H
