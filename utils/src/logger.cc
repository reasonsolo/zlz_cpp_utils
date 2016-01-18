//
// Created by lizhizhou on 2016/1/18.
//

#include "logger.h"
#include "time_utils.h"

ZUTIL_NAMESPACE_BEGIN

string Logger::log_affix_ = "log";

void LogRecord::SetMessage(std::ostringstream& ss) {
    message = ss.str();
}

Logger::Logger(const string& name, uint32_t rotation_size) :
        log_name_(name), rotation_size_(rotation_size) {

}

Logger::~Logger() {

}

void Logger::Log(const LogLevel lvl, const char* file_name, const uint32_t line_num, std::ostringstream& ss) {
    LogRecord record;
    record.level = lvl;
    record.line_num = line_num;
    record.file_name = file_name;
    record.timestamp = TimeUtils::GetTimestampInMS();
    record.SetMessage(ss);

    DoLog(record);
}

Logger* Logger::Create(const LoggerType type, const string& log_name, uint32_t rotation_size) {
    return nullptr;
}

ZUTIL_NAMESPACE_END

