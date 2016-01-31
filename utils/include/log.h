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
        logger->Prepare();\
        stringstream ss; \
        ss << x; \
        logger->Log(LogLevel::DEBUG, __FILE__, __LINE__, ss); \
        logger->ClearUp();\
    } \
} while (0)


#define DEBUG_LOG(x) __DEBUG_LOG(Logger::GetDefaultLogger(), x);

#define __INFO_LOG(logger, x) \
do { \
    if (!logger) { \
        std::cerr << x; \
    } else if (logger->IsLevelEnabled(LogLevel::INFO)) { \
        logger->Prepare();\
        stringstream ss; \
        ss << x; \
        logger->Log(LogLevel::INFO, __FILE__, __LINE__, ss); \
        logger->ClearUp();\
    } \
} while (0)


#define INFO_LOG(x) __INFO_LOG(Logger::GetDefaultLogger(), x);

#define __WARN_LOG(logger, x) \
do { \
    if (!logger) { \
        std::cerr << x; \
    } else if (logger->IsLevelEnabled(LogLevel::WARN)) { \
        logger->Prepare();\
        stringstream ss; \
        ss << x; \
        logger->Log(LogLevel::WARN, __FILE__, __LINE__, ss); \
        logger->ClearUp();\
    } \
} while (0)


#define WARN_LOG(x) __WARN_LOG(Logger::GetDefaultLogger(), x);

#define __ERROR_LOG(logger, x) \
do { \
    if (!logger) { \
        std::cerr << x; \
    } else if (logger->IsLevelEnabled(LogLevel::ERROR)) { \
        logger->Prepare();\
        stringstream ss; \
        ss << x; \
        logger->Log(LogLevel::ERROR, __FILE__, __LINE__, ss); \
        logger->ClearUp();\
    } \
} while (0)

#define ERROR_LOG(x) __ERROR_LOG(Logger::GetDefaultLogger(), x);

// die here
#define __FATAL_LOG(logger, x) \
do { \
    if (!logger) { \
        std::cerr << x; \
    } else if (logger->IsLevelEnabled(LogLevel::FATAL)) { \
        logger->Prepare();\
        stringstream ss; \
        ss << x; \
        logger->Log(LogLevel::FATAL, __FILE__, __LINE__, ss); \
        logger->ClearUp();\
    } \
    assert(__LINE__ == -1); \
} while (0)


#define FATAL_LOG(x) __FATAL_LOG(Logger::GetDefaultLogger(), x);



ZUTIL_NAMESPACE_END

#endif //ZUTILS_LOG_H
