//
// Created by zlz on 2016/1/11.
//

#ifndef ZUTILS_LOCK_H
#define ZUTILS_LOCK_H

#include "common.h"
#include <pthread.h>

ZUTIL_NAMESPACE_BEGIN


class Lock: public NoCopy {

public:
    Lock();

    ~Lock();

    void Lockup();

    void Unlock();

    bool TryLock();

    pthread_mutex_t& lock();

private:
    pthread_mutex_t lock_;
};

class ScopedMutex : private NoCopy {
public:
    ScopedMutex(pthread_mutex_t* lock);
    ScopedMutex(Lock& lock);
    ~ScopedMutex();

private:
    pthread_mutex_t* lock_;
};

class ScopedReadLock : private NoCopy {
public:
    ScopedReadLock(pthread_rwlock_t* lock);

    ~ScopedReadLock();

private:
    pthread_rwlock_t* lock_;
};

class ScopedWriteLock : private NoCopy {
public:
    ScopedWriteLock(pthread_rwlock_t* lock);

    ~ScopedWriteLock();

private:
    pthread_rwlock_t* lock_;
};


ZUTIL_NAMESPACE_END

#endif //ZUTILS_LOCK_H
