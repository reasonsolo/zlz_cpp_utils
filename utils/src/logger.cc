//
// Created by lizhizhou on 2016/1/18.
//
#include "logger.h"
#include "file_utils.h"
#include "string_utils.h"
#include "lock.h"
#include "time_utils.h"
#include "sync_logger.h"
#include "sys_utils.h"

ZUTIL_NAMESPACE_BEGIN

string Logger::log_suffix_ = "log";
LoggerType Logger::default_logger_type_ = LoggerType::kSync;
pthread_rwlock_t Logger::loggers_lock_ = PTHREAD_RWLOCK_INITIALIZER;
map<string, Logger*> Logger::loggers_;
pthread_mutex_t Logger::default_logger_lock_ = PTHREAD_MUTEX_INITIALIZER;
Logger* Logger::default_logger_ = NULL;

Logger::Logger(const string& name, int32_t rotation_size) :
        path_(), base_filename_(name), rotation_size_(rotation_size) {

}

Logger::~Logger() {

}

void Logger::Log(LogLevel lvl, const char* file_name, const int32_t line_num, std::stringstream& ss) {
    LogRecord record(ss);
    record.level = lvl;
    record.line_num = line_num;
    record.file_name = file_name;
    record.timestamp = TimeUtils::GetTimestampInMS();
    DoLog(record);
}

bool Logger::IsLevelEnabled(LogLevel level) {
    return level_ >= level;
}

Logger* Logger::Create(LoggerType type, const string& log_name, uint32_t rotation_size) {
    ScopedWriteLock lock(&loggers_lock_);
    Logger* logger_ = nullptr;
    switch (type) {
        case LoggerType::kAsync:
        case LoggerType::kSync:
        default:
            logger_ = new SyncLogger(log_name, rotation_size);
    }
    return logger_;
}

Logger* Logger::GetLogger(const string& log_name) {

    {
        ScopedReadLock lock(&loggers_lock_);
        auto it = loggers_.find(log_name);
        if (it != loggers_.end()) {
            return it->second;
        }
    }
    {
        ScopedWriteLock lock(&loggers_lock_);
        Logger* logger = Create(default_logger_type_, log_name, kDefaultRotationSize);
        TRACE_LOG("create logger " << log_name << " " << logger);
        loggers_.insert(make_pair(log_name, logger));
        return logger;
    }
}

Logger* Logger::GetDefaultLogger() {
    if (!default_logger_) {
        ScopedMutex lock(&default_logger_lock_);
        if (!default_logger_) {
            TRACE_LOG(SysUtils::GetProcShortName());
            default_logger_ = GetLogger(SysUtils::ReplaceSuffix(SysUtils::GetProcShortName(), log_suffix_));
            string path, name;
            FileUtils::GetDirectoryAndFile(__FILE__, path, name);
            if (default_logger_) {
                default_logger_->set_path(path);
            }
        }
    }
    return default_logger_;
}


string Logger::GetLogLevelString(LogLevel lvl) {
    static const char* kLvlStrings[] = {
            "ALL",
            "TRACE",
            "DEBUG",
            "INFO",
            "WARN",
            "ERROR",
            "FATAL",
            "OFF"
    };
    return "ALL";
    TRACE_LOG("get lvl str "  << static_cast<uint32_t>(lvl) << kLvlStrings[static_cast<uint32_t>(lvl)]);
    return kLvlStrings[static_cast<uint32_t>(lvl)];
}


string Logger::GetRotationFilename() const {
    return GetCurrentFilename() + "." + StringUtils::ToString(file_count_);
}

string Logger::GetCurrentFilename() const {
    return FileUtils::ConcatPathFile(path_, base_filename_);
};


ZUTIL_NAMESPACE_END

