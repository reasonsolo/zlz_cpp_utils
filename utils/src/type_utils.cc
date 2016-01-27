//
// Created by zlz on 2016/1/27.
//

#include "type_utils.h"

ZUTIL_NAMESPACE_BEGIN

template<>
string get_typename(const string& t) {
    // template specialization: rtti will give ugly answer
    return "string";
}

template<>
bool AnyCast(const string& from, int32_t& to) {
    return NumericUtils::String2Int32(from, to);
}

template<>
bool AnyCast(const string& from, int64_t& to) {
    return NumericUtils::String2Int64(from, to);
}

template<>
bool AnyCast(const string& from, uint32_t& to) {
    return NumericUtils::String2UInt32(from, to);
}

template<>
bool AnyCast(const string& from, uint64_t& to) {
    return NumericUtils::String2UInt64(from, to);
}

template<>
bool AnyCast(const string& from, double& to) {
    return NumericUtils::String2Double(from, to);
}

template<>
bool AnyCast(const string& from, bool& to) {
    int64_t tmp = 0;
    if (NumericUtils::String2Int64(from, tmp)) {
        to = implict_cast<bool>(tmp);
        return true;
    }
    if (from == "true") {
        to = true;
        return true;
    }
    if (from == "false") {
        to = false;
        return true;
    }
    return false;
};

ZUTIL_NAMESPACE_END