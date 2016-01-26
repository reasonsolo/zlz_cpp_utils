//
// Created by zlz on 2016/1/12.
//

#ifndef ZUTILS_INI_PARSER_H
#define ZUTILS_INI_PARSER_H

#include "common.h"
#include "lock.h"
#include "type_utils.h"
#include "zthread.h"

ZUTIL_NAMESPACE_BEGIN

class IniParser {
    typedef std::function<void(IniParser*)> ParseCallback;
public:
    /*
     * check_interval: in seconds, if > 0, the check thread will
     * periodically check and reload the ini file
     */
    IniParser(const string& filename, bool case_sensitive = false, int32_t check_interval = 0);

    ~IniParser();

    /*
     * add a callback invoked when the parsing succeeds
     */
    void AddCallback(const ParseCallback& cb) {
        callbacks_.push_back(cb);
    }

    bool Parse();

    template<typename T>
    T Get(const string& section, const string& name, const T& def) {
        T tmp;
        ScopedReadLock lock(&data_lock_);
        auto it = data_.find(MakeKey(section, name));
        if (it != data_.end() && TypeUtils::AnyCast<string, T>(it->second, tmp)) {
            return tmp;
        }
        return def;
    }

private:
    static void Update(IniParser* parser);

    string MakeKey(const string& section, const string& name);

    string filename_;
    bool case_sensitive_;
    map<string, string> data_;
    vector<ParseCallback> callbacks_;

    pthread_rwlock_t data_lock_;
    int32_t check_interval_;
    Thread  check_thread_;
};


ZUTIL_NAMESPACE_END

#endif //ZUTILS_INI_PARSER_H
