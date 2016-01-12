//
// Created by zlz on 2016/1/12.
//

#ifndef ZUTILS_NUMRIC_UTILS_H
#define ZUTILS_NUMRIC_UTILS_H

#include "common.h"

ZUTIL_NAMESPACE_BEGIN

class NumericUtils {
public:
    bool String2Int32(const string& str, int32_t& v);
    bool String2UInt32(const string& str, uint32_t& v);
    bool String2Int64(const string& str, int64_t& v);
    bool String2UInt64(const string& str, int64_t& v);
    bool String2Double(const string& str, double& v);
};

ZUTIL_NAMESPACE_END


#endif //ZUTILS_NUMRIC_UTILS_H
