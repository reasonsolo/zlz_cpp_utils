//
// Created by zlz on 2016/1/17.
//

#include "event_loop_thread_pool.h"

ZUTIL_NET_NAMESPACE_BEGIN

EventLoopThreadPool::EventLoopThreadPool(uint32_t size): pool_size_(size) {

}

EventLoopThreadPool::~EventLoopThreadPool() {
    Clearup();
}

void EventLoopThreadPool::Init(const LoopInitCallback& init_cb) {
    init_cb_ = init_cb;
}


ZUTIL_NET_NAMESPACE_END