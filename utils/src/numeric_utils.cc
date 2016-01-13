//
// Created by lizhizhou on 2016/1/13.
//

#include "numeric_utils.h"

ZUTIL_NAMESPACE_BEGIN


bool NumericUtils::String2Int32(const string& str, int32_t& v) {
    char* end = nullptr;
    int32_t tmp = strtol(str.c_str(), &end, 0);
    if (end == str.c_str()) {
        return false;
    }
    v = tmp;
    return true;
}

bool NumericUtils::String2UInt32(const string& str, uint32_t& v) {
    char* end = nullptr;
    uint32_t tmp = strtoul(str.c_str(), &end, 0);
    if (end == str.c_str()) {
        return false;
    }
    v = tmp;
    return true;
}

bool NumericUtils::String2Int64(const string& str, int64_t& v) {
    char* end = nullptr;
    int64_t tmp = strtoll(str.c_str(), &end, 0);
    if (end == str.c_str()) {
        return false;
    }
    v = tmp;
    return true;
}

bool NumericUtils::String2UInt64(const string& str, uint64_t& v) {
    char* end = nullptr;
    uint64_t tmp = strtoull(str.c_str(), &end, 0);
    if (end == str.c_str()) {
        return false;
    }
    v = tmp;
    return true;
}

bool NumericUtils::String2Double(const string& str, double& v) {
    char* end = nullptr;
    double tmp = strtod(str.c_str(), &end);
    if (end == str.c_str()) {
        return false;
    }
    v = tmp;
    return false;
}

bool NumericUtils::Int322String(const int32_t v, string& str) {
    return false;
}

bool NumericUtils::Double2String(const double v, string& str) {
    return false;
}

ZUTIL_NAMESPACE_END

