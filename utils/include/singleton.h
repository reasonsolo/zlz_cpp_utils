//
// Created by zlz on 2016/1/13.
//

#ifndef ZUTILS_SINGLETON_H
#define ZUTILS_SINGLETON_H

#include "common.h"

ZUTIL_NAMESPACE_BEGIN

/*
 * singleton holder
 */
class Singleton {
public:
    template <typename T>
    static T* instance() {
        static T instance;
        return &instance;
    }
};

/*
 * thread local singleton
 */

template <typename T>
class ThreadSingleton: public NoCopy {
public:
    static T* instance() {
        if (!instance_) {
            instance_ = new T;
        }
        return instance_;
    }
private:
    static __thread T* instance_;
};

template <typename T>
T* ThreadSingleton<T>::instance_ = nullptr;



ZUTIL_NAMESPACE_END

#endif //ZUTILS_SINGLETON_H
