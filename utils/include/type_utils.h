//
// Created by zlz on 2016/1/26.
//

#ifndef ZUTILS_TYPE_UTILS_H
#define ZUTILS_TYPE_UTILS_H

#include "common.h"
#include "numeric_utils.h"
#include <cxxabi.h>
#include <typeinfo>

ZUTIL_NAMESPACE_BEGIN

class TypeUtils {
public:

    template<typename T>
    static string GetTypename(const T& t) const {
        // use typeid but avoid rtti
        // demangle in libstdc++ converts a type name to a human-readable name
        int32_t status = 0;
        char* p = abi::__cxa_demangle(typeid(t).name(), 0, 0, &status);
        std::string ret(p);
        free(p);
        return ret;
    }

    template<>
    static string GetTypename(const string& t) const {
        // template specialization: rtti will give ugly answer
        return "string";
    }

    template<typename From, typename To>
    static bool AnyCast(const From& from, To& to) {
        stringstream ss;
        return ss << from && ss >> to && ss.eof();
    };

    template<>
    static bool AnyCast(const string& from, int32_t& to) {
        return NumericUtils::String2Int32(from, to);
    }

    template<>
    static bool AnyCast(const string& from, int64_t& to) {
        return NumericUtils::String2Int64(from, to);
    }

    template<>
    static bool AnyCast(const string& from, uint32_t& to) {
        return NumericUtils::String2UInt32(from, to);
    }

    template<>
    static bool AnyCast(const string& from, uint64_t& to) {
        return NumericUtils::String2UInt64(from, to);
    }

    template<>
    static bool AnyCast(const string& from, double& to) {
        return NumericUtils::String2Double(from, to);
    }

    template<>
    static bool AnyCast(const string& from, bool& to) {
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
    }
};

ZUTIL_NAMESPACE_END

#endif //ZUTILS_TYPE_UTILS_H
