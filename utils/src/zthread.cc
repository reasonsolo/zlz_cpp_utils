//
// Created by zlz on 2016/1/11.
//
#include <sys/time.h>
#include <w32api/_timeval.h>
#include "zthread.h"

ZUTIL_NAMESPACE_USE;

ThreadCond::ThreadCond() {
    pthread_cond_init(&cond_, NULL);
}

ThreadCond::~ThreadCond() {
    pthread_cond_destroy(&cond_);
}

void Thread::Start() {
    Prepare();
    state_ = kRunning;
    pthread_create(&thread_id_, &attr_, ThreadRoutine, reinterpret_cast<void*>(this));
}

void Thread::Stop(bool wait_stop) {
    Cleanup();
    stop_ = true;
    if (state_ == kSleeping) {
        DoWakeup();
    }
    state_ = kStopped;
    if (wait_stop) {
        Join();
    }
}

void Thread::Kill() {
    Cleanup();
    pthread_kill(thread_id_, 9);
}

void Thread::SetStackSize(size_t size) {
    pthread_attr_setstacksize(&attr_, size);
}

pthread_t Thread::GetThreadId() {
    return pthread_self();
}

pthread_t Thread::thread_id() {
    return thread_id_;
}

void Thread::Prepare() {

}

void Thread::Cleanup() {

}

void* Thread::ThreadRoutine(void* arg) {
    Thread* self = reinterpret_cast<Thread*>(arg);
    self->Run();
    return nullptr;
}

Thread::Thread() : state_(kStopped), thread_id_(0), stop_(false) {
    pthread_attr_init(&attr_);
}

Thread::~Thread() {
    pthread_attr_destroy(&attr_);
}

void Thread::Detach() {
    is_detached_ = true;
    pthread_detach(thread_id_);
}

void Thread::Join() {
    // this thread cannot invoke join
    if (Thread::GetThreadId() != thread_id() && !is_detached_) {
        pthread_join(thread_id_, NULL);
    }
}

void Thread::Wakeup() {
    if (GetThreadId() != thread_id()) {
        DoWakeup();
    }
}

void Thread::DoWakeup() {
    ScopedMutex mutex(lock_);
    state_ = kRunning;
    cond_.Signal();
}

void Thread::Sleep(int32_t time_ms) {
    if (GetThreadId() == thread_id() && state_ == kRunning) {
        ScopedMutex mutex(lock_);
        state_ = kSleeping;
        cond_.Wait(lock_, time_ms);
        state_ = kRunning;
    }
}

bool ThreadCond::Wait(Lock& lock, int64_t wait_time_ms) {
    // cond wait will unlock mutex
    int32_t ret = 0;
    if (wait_time_ms == 0) {
        ret = pthread_cond_wait(&cond_, &lock.lock());
    } else {
        struct timespec abstime;
        struct timeval tv;
        gettimeofday(&tv, nullptr);
        abstime.tv_sec = tv.tv_sec + wait_time_ms / 1000;
        abstime.tv_nsec = tv.tv_usec * 1000 + (wait_time_ms % 1000) * 1000000;

        // handle abstime.tv_nsec overflow
        if (abstime.tv_nsec > 1000000000L) {
            abstime.tv_sec++;
            abstime.tv_nsec %= 1000000000L;
        }

        ret = pthread_cond_timedwait(&cond_, &lock.lock(), &abstime);
    }

    return ret == 0;
}

void ThreadCond::Signal() {
    pthread_cond_signal(&cond_);
}
