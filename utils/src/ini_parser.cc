//
// Created by zlz on 2016/1/26.
//

#include "ini_parser.h"

ZUTIL_NAMESPACE_BEGIN

IniParser::IniParser(const string& filename, bool case_sensitive, int32_t check_interval) :
        filename_(filename), case_sensitive_(case_sensitive), check_interval_(check_interval),
        check_thread_(std::bind(Update, this)) {
    pthread_rwlock_init(&data_lock_, nullptr);
}

IniParser::~IniParser() {
    if (check_thread_.IsRunning()) {
        check_thread_.Stop();
    }
}

bool IniParser::Parse() {
    bool success = true;
    // TODO: do parsing
    if (success) {
        for (auto& cb: callbacks_) {
            cb();
        }
    }
    if (check_interval_ > 0) {
        check_thread_.Start();
    }
    return false;
}

string IniParser::MakeKey(const string& section, const string& name) {
    string key = section + name;
    if (!case_sensitive_) {
        std::transform(key.begin(), key.end(), key.begin(), ::tolower);
    }
    return key;
}


ZUTIL_NAMESPACE_END