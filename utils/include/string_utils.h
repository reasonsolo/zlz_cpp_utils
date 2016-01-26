//
// Created by zlz on 2016/1/11.
//

#ifndef ZUTILS_STRING_UTILS_H
#define ZUTILS_STRING_UTILS_H

#include "common.h"

ZUTIL_NAMESPACE_BEGIN

class StringUtils {
public:
    /*
     * replace any occurrence of some string to another in a string
     */
    static string ReplaceAll(const string& source, const string& from, const string& to) {
        string new_string;
        new_string.reserve(source.length());  // avoids a few memory allocations
        string::size_type last_pos = 0;
        string::size_type find_pos = string::npos;
        while ((find_pos = source.find(from, last_pos)) != string::npos) {
            new_string.append(source, last_pos, find_pos - last_pos);
            new_string += to;
            last_pos = find_pos + from.length();
        }
        // care for the rest after last occurrence
        new_string += source.substr(last_pos);
        return new_string;
    }

    /*
     * split string to vector
     */
    static size_t Split(const string& source, const string& delim, vector<string>& tokens, bool skip_empty = true);

    static size_t Split(const string& source, const string& delim, const string& val_delim,
                      map<string, string>& tokens);

    static char* AsArray(string& str) {
        return &str[0];
    }

    static const char* AsArray(const string& str) {
        return &str[0];
    }

    /*
     * silly stringstream usage
     * use string::append if possible
     * TODO: specialize it with type like int and double
     */
    template<typename T>
    static string ToString(const T& t) {
        stringstream ss;
        ss << t;
        return ss.str();
    }


    template<typename T0, typename T1>
    static string ToString(const T0& t0, const T1& t1) {
        stringstream ss;
        ss << t0 << t1;
        return ss.str();
    }

    template<typename T0, typename T1, typename T2>
    static string ToString(const T0& t0, const T1& t1, const T2& t2) {
        stringstream ss;
        ss << t0 << t1 << t2;
        return ss.str();
    }

    template<typename T0, typename T1, typename T2, typename T3>
    static string ToString(const T0& t0, const T1& t1, const T2& t2, const T3& t3) {
        stringstream ss;
        ss << t0 << t1 << t2 << t3;
        return ss.str();
    }

    template<typename T0, typename T1, typename T2, typename T3, typename T4>
    static string ToString(const T0& t0, const T1& t1, const T2& t2, const T3& t3, const T4& t4) {
        stringstream ss;
        ss << t0 << t1 << t2 << t3 << t4;
        return ss.str();
    }

    template<typename T0, typename T1, typename T2, typename T3, typename T4, typename T5>
    static string ToString(const T0& t0, const T1& t1, const T2& t2, const T3& t3, const T4& t4, const T5& t5) {
        stringstream ss;
        ss << t0 << t1 << t2 << t3 << t4 << t5;
        return ss.str();
    }

    // TODO: add more ToString
};

ZUTIL_NAMESPACE_END

#endif //ZUTILS_STRING_UTILS_H
