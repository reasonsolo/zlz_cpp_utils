//
// Created by lizhizhou on 2016/2/16.
//

#ifndef ZUTILS_EVHTTP_THREAD_H
#define ZUTILS_EVHTTP_THREAD_H

#include "common.h"
#include <event2/event.h>
#include "string_utils.h"

ZUTIL_EV_NAMESPACE_BEGIN

class Sensor;
class Thread;

typedef std::function<void()> EvCallback;

struct EvThreadCmd {
    uint8_t stop;
    EvCallback cb;
};

class EvThread {
public:

    EvThread();

    ~EvThread();

    void Init();

    void Clearup();

    void Start();

    void Stop();

    void Defer(EvCallback cb);

    string ToString() const {
        return StringUtils::ToString("EvThread-", base_);
    }

private:
    void Run();


    static void HandleCmd(evutil_socket_t fd, int16_t which, void* arg);

    event_base* base_;
    event* event_;

    Sensor* sensor_;
    Thread* thread_;
};

ZUTIL_EV_NAMESPACE_END

#endif //ZUTILS_EVHTTP_THREAD_H
