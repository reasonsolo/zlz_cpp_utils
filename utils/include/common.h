//
// Created by zlz on 2016/1/11.
//

#ifndef ZUTILS_COMMON_H
#define ZUTILS_COMMON_H

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <sstream>
#include <stdint.h>
#include <algorithm>
#include <cstdlib>
#include <cstring>
#include <utility>


using std::stringstream;
using std::string;
using std::vector;
using std::map;
using std::pair;
using std::make_pair;

#define DEBUG_LOG(x) std::cerr << x << std::endl;

#define ZUTIL_NAMESPACE_BEGIN namespace zutil {
#define ZUTIL_NAMESPACE_END   }
#define ZUTIL_NAMESPACE_USE   using namespace zutil



class NoCopy {
public:
    NoCopy() {}
private:
    NoCopy(const NoCopy& ncpy);
    NoCopy& operator=(const NoCopy& ncpy);
};

#endif //ZUTILS_COMMON_H
