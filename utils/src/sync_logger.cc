//
// Created by zlz on 2016/1/19.
//

#include "sync_logger.h"
#include "time_utils.h"
#include "file_utils.h"

ZUTIL_NAMESPACE_BEGIN

SyncLogger::SyncLogger(const string& name, uint32_t rotation_size) : base_filename_(name),
                                                                     rotation_size_(rotation_size) {

}

SyncLogger::~SyncLogger() {

}

void SyncLogger::DoLog(const LogRecord& record) {
    ScopedMutex lock(&lock_);
    BeforeLog();
    if (fstream_.good()) {
        fstream_ << "[" << GetLogLevelStr(level_) << "]"
                 << TimeUtils::TimestampToString(record.timestamp) << " "
                 << record.file_name << "-" << record.line_num << ": "
                 << record.message.rdbuf() << std::endl;
        fstream_.flush();
    } else {

    }
    AfterLog();
}

void SyncLogger::BeforeLog() {
    if (!fstream_.is_open()) {
        fstream_.open(GetCurrentFilename());
        if (!fstream_.is_open()) {
            handle_sys_error("cannot open log file");
            return;
        }
    }
    if (!fstream_.good()) {
        fstream_.close();
        fstream_.open(GetCurrentFilename());
    }
}

void SyncLogger::AfterLog() {
    if (FileUtils::GetFileSize(GetCurrentFilename()) > rotation_size_) {
        Rotate();
    }
}

void SyncLogger::Rotate() {
    fstream_.close();
    FileUtils::Rename(GetCurrentFilename(), GetRotationFilename());
    file_count_++;
}

ZUTIL_NAMESPACE_END
