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

template<typename T>
string get_typename(const T& t) {
    // use typeid but avoid rtti
    // demangle in libstdc++ converts a type name to a human-readable name
    int32_t status = 0;
    char* p = abi::__cxa_demangle(typeid(t).name(), 0, 0, &status);
    std::string ret(p);
    free(p);
    return ret;
}

/*
 * cannot define a template specialization in headers
 */
template<>
string get_typename(const string& t);

template<typename From, typename To>
bool AnyCast(const From& from, To& to) {
    stringstream ss;
    return ss << from && ss >> to && ss.eof();
};

template<>
bool AnyCast(const string& from, int32_t& to);

template<>
bool AnyCast(const string& from, int64_t& to);

template<>
bool AnyCast(const string& from, uint32_t& to);

template<>
bool AnyCast(const string& from, uint64_t& to);

template<>
bool AnyCast(const string& from, double& to);

template<>
bool AnyCast(const string& from, bool& to);

ZUTIL_NAMESPACE_END

#endif //ZUTILS_TYPE_UTILS_H
