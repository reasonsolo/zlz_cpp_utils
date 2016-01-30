//
// Created by zlz on 2016/1/17.
//

#include "event_loop_thread_pool.h"
#include "event_loop.h"
#include "zthread.h"

ZUTIL_NET_NAMESPACE_BEGIN

EventLoopThreadPool::EventLoopThreadPool():
         loop_index_(0) {

}

EventLoopThreadPool::~EventLoopThreadPool() {
    Destroy();
}

void EventLoopThreadPool::Init() {
    for (uint32_t i = 0; i < thread_size_; i++) {
        EventLoop* loop = new EventLoop();
        event_loops_.push_back(loop);

        Thread* thread = new Thread(std::bind(ThreadFunctorRun, loop));
        thread->set_before_stop(std::bind(ThreadFunctorStop, loop));
        threads_.push_back(thread);
    }
}

void EventLoopThreadPool::Destroy() {
    for (auto& thread: threads_) {
        thread->Stop();
        delete thread;
    }
    for (auto& loop: event_loops_) {
        delete loop;
    }
}

void EventLoopThreadPool::Start() {
    Init();
    for (auto& thread: threads_) {
        thread->Start();
    }
}

void EventLoopThreadPool::Stop() {
    for (auto thread: threads_) {
        thread->Stop();
    }
}

EventLoop* EventLoopThreadPool::GetNextLoop() {
    return event_loops_[loop_index_++ % event_loops_.size()];
}

EventLoop* EventLoopThreadPool::GetLoopByHash(int64_t hash) {
    return event_loops_[static_cast<uint32_t>(hash) % event_loops_.size()];
}

void EventLoopThreadPool::ThreadFunctorRun(EventLoop* loop) {
    loop->Start();
}

void EventLoopThreadPool::ThreadFunctorStop(EventLoop* loop) {
    loop->Stop();
}

ZUTIL_NET_NAMESPACE_END