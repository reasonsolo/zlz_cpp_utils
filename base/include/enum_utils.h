//
// Created by zlz on 2016/1/31.
//

#ifndef ZUTILS_ENUM_UTILS_H
#define ZUTILS_ENUM_UTILS_H

#include "common.h"

ZUTIL_NAMESPACE_BEGIN

// enum <-> string conversion
// from http://codereview.stackexchange.com/a/14315
// This is the type that will hold all the strings.
// Each enumerate type will declare its own specialization.
// Any enum that does not have a specialization will generate a compiler error
// indicating that there is no definition of this variable (as there should be
// be no definition of a generic version).
// example
/*
    enum X {Hi, Lo};
    template<> char const* enumStrings<X>::data[] = {"Hi", "Lo"};
    X a=Hi;
    std::cout << enumToString(a) << "\n";
    std::stringstream line("Lo");
    line >> enumFromString(a);
    std::cout << "A: " << a << " : " << enumToString(a) << "\n";
 */
template<typename T>
struct EnumStrings {
    static char const* data[];
};

// This is a utility type.
// Created automatically. Should not be used directly.
template<typename T>
struct EnumRefHolder {
    T& enum_;

    EnumRefHolder(T& t) : enum_(t) { }
};

template<typename T>
struct EnumConstRefHolder {
    const T& enum_;

    EnumConstRefHolder(const T& t) : enum_(t) { }
};

// The next too functions do the actual work of reading/writtin an
// enum as a string.
template<typename T>
std::ostream& operator<<(std::ostream& str, const EnumConstRefHolder<T>& data) {
    return str << EnumStrings<T>::data[data.enum_];
}

template<typename T>
std::istream& operator>>(std::istream& str, EnumRefHolder<T>& data) {
    string value;
    str >> value;

    // These two can be made easier to read in C++11
    // using std::begin() and std::end()
    //
    static auto begin = std::begin(EnumStrings<T>::data);
    static auto end = std::end(EnumStrings<T>::data);

    auto find = std::find(begin, end, value);
    if (find != end) {
        data.enum_ = static_cast<T>(std::distance(begin, find));
    }
    return str;
}


// This is the public interface:
// use the ability of function to deuce their template type without
// being explicitly told to create the correct type of enumRefHolder<T>
template<typename T>
EnumConstRefHolder<T> EnumToString(const T& e) { return EnumConstRefHolder<T>(e); }

template<typename T>
EnumRefHolder<T> EnumFromString(T& e) { return EnumRefHolder<T>(e); }

ZUTIL_NAMESPACE_END

#endif //ZUTILS_ENUM_UTILS_H
