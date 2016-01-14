//
// Created by lizhizhou on 2016/1/14.
//

#ifndef ZUTILS_ATOMIC_H
#define ZUTILS_ATOMIC_H

#include "common.h"

ZUTIL_NAMESPACE_BEGIN

template <typename T>
class AtomicInt: public NoCopy {
public:
    AtomicInt(T val): val_(val) {}
    ~AtomicInt() {}

    T Get() const {
        return val_;
    }

    T GetAndAdd(T x) {
        return __sync_fetch_and_add(&val_, x);
    }

    T AddAndGet(T x) {
        return __sync_fetch_and_add(&val_, x) + x;
    }

    T GetAndSet(T x) {
        return __sync_lock_test_and_set(&val_, x);
    }

    T Incr() {
        return AddAndGet(1);
    }

    T Decr() {
        return AddAndGet(-1);
    }
private:
    volatile T val_;
};


ZUTIL_NAMESPACE_END


#endif //ZUTILS_ATOMIC_H
