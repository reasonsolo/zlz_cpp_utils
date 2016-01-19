//
// Created by zlz on 2016/1/19.
//

#ifndef ZUTILS_SYS_UTILS_H
#define ZUTILS_SYS_UTILS_H

#include "common.h"
#include "errno.h"

ZUTIL_NAMESPACE_BEGIN

class SysUtils {
public:
    static string GetProcFullName();

    static string GetProcShortName();

    static string ReplaceSuffix(const string& filename, const string& suffix);
};

ZUTIL_NAMESPACE_END

#endif //ZUTILS_SYS_UTILS_H
