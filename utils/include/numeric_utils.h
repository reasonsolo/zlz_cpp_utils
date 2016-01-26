//
// Created by zlz on 2016/1/12.
//

#ifndef ZUTILS_NUMRIC_UTILS_H
#define ZUTILS_NUMRIC_UTILS_H

#include "common.h"

ZUTIL_NAMESPACE_BEGIN

class NumericUtils {
public:
    static bool String2Int32(const string& str, int32_t& v);

    static bool String2UInt32(const string& str, uint32_t& v);

    static bool String2Int64(const string& str, int64_t& v);

    static bool String2UInt64(const string& str, uint64_t& v);

    static bool String2Double(const string& str, double& v);

    static string Int322String(const int32_t v);

    static string Double2String(const double v);
};

ZUTIL_NAMESPACE_END


#endif //ZUTILS_NUMRIC_UTILS_H
