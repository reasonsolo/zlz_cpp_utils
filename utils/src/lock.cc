//
// Created by zlz on 2016/1/11.
//
#include "lock.h"

ZUTIL_NAMESPACE_USE;


Lock::Lock() {
    pthread_mutex_init(&lock_, nullptr);
}

Lock::~Lock() {
}

pthread_mutex_t& Lock::lock() {
    return lock_;
}

void Lock::Lockup() {
    pthread_mutex_lock(&lock_);
}

void Lock::Unlock() {
    pthread_mutex_unlock(&lock_);
}

bool Lock::TryLock() {
    return pthread_mutex_trylock(&lock_) == 0;
}

ScopedReadLock::ScopedReadLock(pthread_rwlock_t* lock) {
    lock_ = lock;
    pthread_rwlock_rdlock(lock_);
}

ScopedReadLock::~ScopedReadLock() {
    pthread_rwlock_unlock(lock_);
}


ScopedWriteLock::ScopedWriteLock(pthread_rwlock_t* lock) {
    lock_ = lock;
    pthread_rwlock_wrlock(lock_);
}

ScopedWriteLock::~ScopedWriteLock() {
    pthread_rwlock_unlock(lock_);
}

ScopedMutex::ScopedMutex(pthread_mutex_t* lock) {
    lock_ = lock;
    pthread_mutex_lock(lock_);
}

ScopedMutex::~ScopedMutex() {
    pthread_mutex_unlock(lock_);
}

ScopedMutex::ScopedMutex(Lock& lock) {
    lock_ = &lock.lock();
    pthread_mutex_lock(lock_);
}
