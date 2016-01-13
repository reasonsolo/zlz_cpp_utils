//
// Created by zlz on 2016/1/11.
//

#ifndef ZUTILS_THREAD_H
#define ZUTILS_THREAD_H

#include "common.h"

ZUTIL_NAMESPACE_BEGIN
/*
 * must use a heap thread object but not any stack object
 */
class Thread : private NoCopy {
public:
    Thread();

    virtual ~Thread();

    void Start();

    void Stop(bool wait_stop);

    /*
     * send a kill signal
     */
    void Kill();

    void Detach();

    void Join();

    /*
     * implement this method in sub-class
     */
    virtual void Run() = 0;


    /*
     * use this before start
     */
    void SetStackSize(size_t size);

    static uint32_t GetThreadId();

    uint32_t thread_id();

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

    pthread_t thread_id_;
    pthread_attr_t attr_;
    volatile bool is_detached_;
    volatile bool stop_;

};


class ThreadCond : public NoCopy {
public:
    ThreadCond();
    ~ThreadCond();


private:
    pthread_cond_t cond_;
};


ZUTIL_NAMESPACE_END

#endif //ZUTILS_THREAD_H
