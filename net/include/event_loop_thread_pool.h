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

    EventLoopThreadPool();

    ~EventLoopThreadPool();

    string ToString() const;

    EventLoop* GetNextLoop();

    EventLoop* GetLoopByHash(int64_t hash);

    void set_thread_size(uint32_t size) {
        thread_size_ = size;
    }

    void Start();

    void Stop();

protected:
    void Init();

    void Destroy();

    void ThreadFunctorRun(uint32_t index);

    void ThreadFunctorStop(uint32_t index);

private:
    uint32_t thread_size_;
    uint32_t loop_index_;
    vector<EventLoop*> event_loops_;
    vector<Thread*> threads_;
};

ZUTIL_NET_NAMESPACE_END

#endif //ZUTILS_EVENT_LOOP_THREAD_POLL_H
