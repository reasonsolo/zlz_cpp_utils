//
// Created by zlz on 2016/1/11.
//
#include "zthread.h"

ZUTIL_NAMESPACE_USE;

void Thread::Start() {

    Prepare();

}

void Thread::Stop(bool wait_stop) {
    Cleanup();
    stop_ = true;
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

uint32_t Thread::GetThreadId() {
    return static_cast<int32_t>(pthread_self());
}

uint32_t Thread::thread_id() {
    return static_cast<int32_t>(thread_id_);
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

Thread::Thread() : thread_id_(0), stop_(false) {
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
