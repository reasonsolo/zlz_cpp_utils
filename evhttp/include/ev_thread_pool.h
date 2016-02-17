//
// Created by lizhizhou on 2016/2/16.
//

#ifndef ZUTILS_EV_THREAD_POOL_H
#define ZUTILS_EV_THREAD_POOL_H

#include "common.h"
#include "ev_thread.h"

ZUTIL_EV_NAMESPACE_BEGIN

class EvThreadPool {
public:
    EvThreadPool(uint32_t size);
    ~EvThreadPool();

    void Start();
    void Stop();

    void Defer(EvCallback cb);
private:
    void Init();
    void Clearup();

    uint32_t size_;
    uint32_t next_index_;
    vector<EvThread*> threads_;
};

ZUTIL_EV_NAMESPACE_END

#endif //ZUTILS_EV_THREAD_POOL_H
