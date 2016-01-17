//
// Created by zlz on 2016/1/17.
//

#ifndef ZUTILS_EVENT_LOOP_THREAD_POLL_H
#define ZUTILS_EVENT_LOOP_THREAD_POLL_H

#include "common.h"
#include "callbacks.h"

ZUTIL_NET_NAMESPACE_BEGIN

class EventLoop;

NO_COPY_CLASS(EventLoopThreadPool) {
public:
    typedef std::function<void(EventLoop*)> LoopInitCallback;

    EventLoopThreadPool(uint32_t size);

    ~EventLoopThreadPool();

    void Init(const LoopInitCallback& init_cb);

    void Clearup();

    string ToString() const;

    EventLoop* GetNextLoop();

    void Start();

    void Stop();

private:
    uint32_t pool_size_;
    LoopInitCallback init_cb_;
    vector<EventLoop*> event_loops_;
    vector<Thread*> threads_;
};

ZUTIL_NET_NAMESPACE_END

#endif //ZUTILS_EVENT_LOOP_THREAD_POLL_H
