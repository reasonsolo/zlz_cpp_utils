//
// Created by zlz on 2016/1/11.
//

#ifndef ZUTILS_LOGGER_H
#define ZUTILS_LOGGER_H

#include "common.h"

ZUTIL_NAMESPACE_BEGIN

/*
 * log level, low to high
 */
enum class LogLevel {
    ALL = 0,
    TRACE,
    DEBUG,
    INFO,
    WARN,
    ERROR,
    FATAL,
    OFF
};

struct LogRecord {
    LogLevel level;
    string file_name;
    uint32_t line_num;
    int64_t timestamp;
    string message;

    void SetMessage(std::ostringstream& ss);
};

enum class LoggerType {
    kNone,
    kSync,
    kAsync
};

const uint32_t kDefaultRotationSize = 100 * 1024 * 1024; // 100M
/*
 * abstract interface
 */
class Logger {
public:
    Logger(const string& name, uint32_t rotation_size = kDefaultRotationSize);
    virtual ~Logger();

    Logger* Create(const LoggerType type, const string& log_name, uint32_t rotation_size);

    virtual void Log(const LogLevel lvl, const char* file_name, const uint32_t line_num, std::ostringstream& ss);

protected:
    virtual void DoLog(const LogRecord& record) = 0;

    string   log_name_;
    uint32_t rotation_size_;

    static string log_affix_;
};

class LogManager {
public:

protected:

};

ZUTIL_NAMESPACE_END

#endif //ZUTILS_LOGGER_H
