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
    int32_t line_num;
    int64_t timestamp;
    std::stringstream& message;

    LogRecord(std::stringstream& ss) : message(ss) {

    }
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
    Logger() { }

    Logger(const string& name, int32_t rotation_size = kDefaultRotationSize);

    virtual ~Logger();

    void set_path(const string& path) {
        path_ = path;
    }

    void set_level(const LogLevel level) {
        level_ = level;
    }

    bool IsLevelEnabled(LogLevel type);

    virtual void Log(LogLevel lvl, const char* file_name, const int32_t line_num, std::stringstream& ss);

    static Logger* Create(LoggerType type, const string& log_name, uint32_t rotation_size);

    static Logger* GetLogger(const string& name);

    static Logger* GetDefaultLogger();

    static string GetLogLevelString(LogLevel lvl);

    virtual void Prepare() {};

    virtual void ClearUp() {};

    LogLevel level() const {
        return level_;
    }

    string path() const {
        return path_;
    }

    string GetCurrentFilename() const;

    string GetRotationFilename() const;

protected:
    virtual void DoLog(const LogRecord& record) = 0;

    LogLevel level_;
    string path_;
    string base_filename_;
    int32_t rotation_size_;
    volatile uint32_t file_count_;

    static string               log_suffix_;
    static LoggerType           default_logger_type_;
    static pthread_rwlock_t     loggers_lock_;
    static map<string, Logger*> loggers_;
    static pthread_mutex_t      default_logger_lock_;
    static Logger*              default_logger_;

};


ZUTIL_NAMESPACE_END

#endif //ZUTILS_LOGGER_H
