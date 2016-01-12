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
    class ZThread: private NoCopy {
    public:
        virtual void Start();

        virtual void Stop();
        /*
         * implement this method in sub-class
         */
        virtual void Run() = 0;

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

        pthread_t      id_;
        pthread_attr_t attr_;
        volatile bool  stop_;

    };


ZUTIL_NAMESPACE_END

#endif //ZUTILS_THREAD_H
