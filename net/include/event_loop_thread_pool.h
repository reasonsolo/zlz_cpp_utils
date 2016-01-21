//
// Created by zlz on 2016/1/17.
//

#ifndef ZUTILS_EVENT_LOOP_THREAD_POLL_H
#define ZUTILS_EVENT_LOOP_THREAD_POLL_H

#include "common.h"
#include "callbacks.h"
#include "zthread.h"

ZUTIL_NET_NAMESPACE_BEGIN

class EventLoop;


NO_COPY_CLASS(EventLoopThreadPool) {
public:
    typedef std::function<void(EventLoop*)> LoopInitCallback;

    EventLoopThreadPool(uint32_t size);

    ~EventLoopThreadPool();

    void Init(const LoopInitCallback& init_cb);

    void Destroy();

    string ToString() const;

    EventLoop* GetNextLoop();

    EventLoop* GetLoopByHash(int64_t hash);

    void Start();

    void Stop();

protected:
    static void ThreadFunctorRun(EventLoop* loop);
    static void ThreadFunctorStop(EventLoop* loop);

private:
    uint32_t pool_size_;
    uint32_t loop_index_;
    LoopInitCallback init_cb_;
    vector<EventLoop*> event_loops_;
    vector<Thread*> threads_;
};

ZUTIL_NET_NAMESPACE_END

#endif //ZUTILS_EVENT_LOOP_THREAD_POLL_H
