//
// Created by lizhizhou on 2016/1/15.
//

#ifndef ZUTILS_CALLBACKS_H
#define ZUTILS_CALLBACKS_H

#include "common.h"
#include <functional>
#include "buffer.h"

ZUTIL_NET_NAMESPACE_BEGIN

class TcpConnection;

typedef std::function<void()> EventCallBack;
typedef std::function<void()> TimerCallback;
typedef std::function<void()> ReadEventCallback;
typedef std::function<void()> WriteEventCallback;
typedef std::function<void()> CloseEventCallback;
typedef std::function<void()> ConnectEventCallback;
typedef std::function<void()> ErrorEventCallback;

typedef std::function<void(TcpConnection*)> ConnectionCallback;
typedef std::function<void(TcpConnection*, Buffer)> OnMessageCallback;
typedef std::function<void()> WriteDoneCallback;
typedef std::function<void(TcpConnection*)> CloseCallback;

ZUTIL_NET_NAMESPACE_END

#endif //ZUTILS_CALLBACKS_H
