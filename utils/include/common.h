//
// Created by zlz on 2016/1/11.
//

#ifndef ZUTILS_COMMON_H
#define ZUTILS_COMMON_H

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <sstream>
#include <stdint.h>
#include <algorithm>

using std::stringstream;
using std::string;
using std::vector;

#define ZUTIL_NAMESPACE_BEGIN namespace zutil {
#define ZUTIL_NAMESPACE_END   }
#define ZUTIL_NAMESPACE_USE   using namespace zutil


/*
 * singleton holder
 */
template <typename T>
class Singleton {
public:
    static T* instance() {
        static T instance;
        return &instance;
    }
};

class NoCopy {
private:
    NoCopy();
    NoCopy(const NoCopy& ncpy);
    NoCopy& operator=(const NoCopy& ncpy);
};

#endif //ZUTILS_COMMON_H
