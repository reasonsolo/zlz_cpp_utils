//
// Created by lizhizhou on 2016/1/18.
//

#ifndef ZUTILS_LOG_H
#define ZUTILS_LOG_H

#include "logger.h"

ZUTIL_NAMESPACE_BEGIN

#define __DEBUG_LOG(logger, x) \
do { \
    if (!logger) { \
        std::cerr << x; \
    } else if (logger->IsLevelEnabled(LogLevel::DEBUG)) { \
        stringstream ss; \
        ss << x; \
        logger->Log(LogLevel::DEBUG, __FILE__, __LINE__, ss); \
    } \
} while (0)


#define DEBUG_LOG(x) __DEBUG_LOG(Logger::GetDefaultLogger(), x);


ZUTIL_NAMESPACE_END

#endif //ZUTILS_LOG_H
