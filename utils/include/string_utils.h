//
// Created by zlz on 2016/1/11.
//

#ifndef ZUTILS_STRING_UTILS_H
#define ZUTILS_STRING_UTILS_H

#include "common.h"

namespace zutils {

class StringUtils {
public:

    template <typename T>
    static string ToString(const T& t) {
        stringstream ss;
        ss << t;
        return ss.str();
    }

private:

};

}

#endif //ZUTILS_STRING_UTILS_H
