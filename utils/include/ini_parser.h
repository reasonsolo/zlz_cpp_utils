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
    typedef map<string, string> Section;
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
    T Get(const string& section, const string& key, const T& def) {
        if (data_) {
            ScopedReadLock lock(&data_lock_);
            string sec = section;
            string k = key;
            CaseTransform(k);
            CaseTransform(sec);
            auto sec_it = data_->find(sec);
            if (sec_it != data_->end()) {
                T tmp;
                auto key_it = sec_it->second.find(k);
                if (key_it != sec_it->second.end() && AnyCast<string, T>(key_it->second, tmp)) {
                    return tmp;
                }
            }
        }
        return def;
    }

    void GetSections(vector<string>& sections);

    void GetSectionKeys(const string& name, vector<string>& keys);

    /*
     * TODO: dump data to file
     */
    bool Dump(const string& dump_filename);

    void Stop();

private:
    static void Update(IniParser* self);

    bool DoParse(map<string, Section>& data);

    bool ParseLine(const string& line, string& key, string& val);

    bool ParseSection(const string& line, string& section);

    bool ParseComment(const string& line);

    void CaseTransform(string& str);

    string filename_;
    bool case_sensitive_;
    map<string, Section>* data_;
    vector<ParseCallback> callbacks_;

    pthread_rwlock_t data_lock_;
    int32_t check_interval_;
    Thread  check_thread_;
};


ZUTIL_NAMESPACE_END

#endif //ZUTILS_INI_PARSER_H
