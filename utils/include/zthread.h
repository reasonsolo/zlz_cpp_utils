//
// Created by zlz on 2016/1/11.
//

#ifndef ZUTILS_THREAD_H
#define ZUTILS_THREAD_H

#include "common.h"
#include "lock.h"

ZUTIL_NAMESPACE_BEGIN

class ThreadCond : public NoCopy {
public:
    ThreadCond();

    ~ThreadCond();

    bool Wait(Lock& lock, int64_t wait_time_ms = 0);

    void Signal();

private:
    pthread_cond_t cond_;
};

/*
 * must use a heap thread object but not any stack object
 */
class Thread : private NoCopy {
    typedef std::function<void()> Functor;
public:
    Thread();

    explicit Thread(const Functor& func);

    virtual ~Thread();

    virtual void Start();

    virtual void Stop(bool wait_stop = false);

    virtual bool IsRunning() {
        return state_ != kStopped;
    }

    /*
     * send a kill signal
     */
    void Kill();

    void Detach();

    void Join();

    /*
     * use this before start
     */
    void SetStackSize(size_t size);

    void set_thread_func(const std::function<void()>& func) {
        thread_func_ = func;
    }

    void set_before_start(const Functor& func) {
        before_start_ = func;
    }

    void set_before_stop(const Functor& func) {
        before_stop_ = func;
    }

    static pthread_t GetThreadId();

    static bool IsSelf(pthread_t thread_id) {
        return GetThreadId() == thread_id;
    }

    pthread_t thread_id();

    /*
     * call by other threads
     */
    void Wakeup();

    string ToString() const;

protected:
    /*
     * call in subclass itself
     */
    void DoWakeup();

    void Sleep(int32_t time_ms = 0);

    /*
     * implement this method in sub-class
     */
    virtual void Run() {};

private:
    /*
     * before start
     */
    virtual void Prepare();

    /*
     * after stop
     */
    virtual void Cleanup();

    static void* ThreadRoutine(void* arg);

    enum State {
        kSleeping,
        kWakedup,
        kRunning,
        kStopped
    };

    Functor thread_func_;
    Functor before_start_;
    Functor before_stop_;

    volatile State state_;
    pthread_t thread_id_;
    pthread_attr_t attr_;
    volatile bool is_detached_;
    volatile bool stop_;

    ThreadCond cond_;
    Lock       lock_;

};



ZUTIL_NAMESPACE_END

#endif //ZUTILS_THREAD_H
