//
// Created by zlz on 2016/1/11.
//

#ifndef ZUTILS_COMMON_H
#define ZUTILS_COMMON_H

#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <map>
#include <sstream>
#include <stdint.h>
#include <algorithm>
#include <cstdlib>
#include <cstring>
#include <utility>
#include <cassert>
#include <cstdio>
#include <unistd.h>

/*
 * you should never name a class duplicating the one in STL
 */
using std::list;
using std::stringstream;
using std::string;
using std::vector;
using std::map;
using std::pair;
using std::make_pair;


// use following macros before logger module is available
#define TRACE_LOG(x) std::cerr << x << std::endl;
#define DEBUG_LOG(x) std::cerr << x << std::endl;
#define INFO_LOG(x)  std::cerr << x << std::endl;
#define WARN_LOG(x)  std::cerr << x << std::endl;
#define FATAL_LOG(x) std::cerr << x << std::endl;

#define ZUTIL_NAMESPACE_BEGIN namespace zutil {
#define ZUTIL_NAMESPACE_END   }
#define ZUTIL_NAMESPACE_USE   using namespace zutil

#define ZUTIL_NET_NAMESPACE_BEGIN namespace zutil { namespace net {
#define ZUTIL_NET_NAMESPACE_END   }}
#define ZUTIL_NET_NAMESPACE_USE   using namespace zutil::net

#define handle_sys_error(x) perror(x)

/*
 * implict cast, usage: implict_cast<TO>(from)
 */
template<typename To, typename From>
To implict_cast(const From& from) {
    return from;
}

class NoCopy {
public:
    NoCopy() {}
private:
    // forbidden
    NoCopy(const NoCopy& ncpy);
    NoCopy& operator=(const NoCopy& ncpy);
};

#define NO_COPY_CLASS(x) class x: public NoCopy

#endif //ZUTILS_COMMON_H
