//
// Created by lizhizhou on 2016/2/17.
//

#include "ev_thread_pool.h"

ZUTIL_EV_NAMESPACE_BEGIN

EvThreadPool::EvThreadPool(uint32_t size): size_(size), next_index_(0) {
    Init();
}

EvThreadPool::~EvThreadPool() {
    Stop();
    Clearup();
}

void EvThreadPool::Init() {
    for (uint32_t i = 0; i < size_; i++) {
        EvThread* thread = new EvThread;
        threads_.push_back(thread);
    }
}

void EvThreadPool::Clearup() {
    for (auto thread: threads_) {
        delete thread;
    }
}

void EvThreadPool::Start() {
    for (auto thread: threads_) {
        thread->Init();
        thread->Start();
    }
    // FIXME: enable this?
    // usleep(5000);
}

void EvThreadPool::Stop() {
    for (auto thread: threads_) {
        thread->Stop();
    }
}

void EvThreadPool::Defer(EvCallback cb) {
    if (threads_.size() > 0) {
        threads_[(next_index_++) % threads_.size()]->Defer(cb);
    }
}

ZUTIL_EV_NAMESPACE_END