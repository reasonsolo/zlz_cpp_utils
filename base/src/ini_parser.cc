//
// Created by zlz on 2016/1/26.
//

#include "ini_parser.h"
#include "string_utils.h"
#include "log.h"
#include <fstream>

ZUTIL_NAMESPACE_BEGIN

IniParser::IniParser(const string& filename, bool case_sensitive, int32_t check_interval) :
        filename_(filename), case_sensitive_(case_sensitive), data_(nullptr),
        check_interval_(check_interval),
        check_thread_(std::bind(Update, this)) {
    pthread_rwlock_init(&data_lock_, nullptr);
}

IniParser::~IniParser() {
   Stop();
}

bool IniParser::Parse() {
    map<string, Section>* new_data = new map<string, Section>;

    bool success = DoParse(*new_data);
    if (success) {
        ScopedWriteLock lock(&data_lock_);
        delete data_;
        data_ = new_data;
    }
    if (success) {
        for (auto& cb: callbacks_) {
            if (cb) {
                cb(this);
            }
        }
    }
    if (check_interval_ > 0 && !check_thread_.IsRunning()) {
        check_thread_.Start();
    }
    return success;
}

bool IniParser::DoParse(map<string, Section>& data) {
    data.clear();
    std::ifstream fs(filename_.c_str());
    if (fs.is_open() && fs.good()) {
        string line;
        string current_section = "";
        string key, val;
        while (std::getline(fs, line)) {
            line = StringUtils::Trim(line);
            if (ParseComment(line)) {
                continue;
            }
            if (ParseSection(line, current_section)) {
                if (data.find(current_section) == data.end()) {
                    data.insert(make_pair(current_section, Section()));
                }
                continue;
            }
            if (ParseLine(line, key, val)) {
                data[current_section][key] = val;
                continue;
            }
        }
    } else {
        return false;
    }
    return true;
}

/*
 * just swallow the empty line and comment line
 */
bool IniParser::ParseComment(const string& line) {
    return line.empty() || line[0] == ';' || line[0] == '#';
}

bool IniParser::ParseSection(const string& line, string& section) {
    if (line.front() == '[' && line.back() == ']') {
        section = line.substr(1, line.size() - 2);
        CaseTransform(section);
        return true;
    }
    return false;
}

bool IniParser::ParseLine(const string& line, string& key, string& val) {
    size_t ret = StringUtils::Split(line, "=", key, val);
    CaseTransform(key);
    return ret == 2;
}

void IniParser::Update(IniParser* self) {
    while (!self->check_thread_.IsStopped()) {
        self->check_thread_.Sleep(self->check_interval_ * 1000);
        self->Parse();
    }
}

void IniParser::GetSections(vector<string>& sections) {
    sections.clear();
    ScopedReadLock lock(&data_lock_);
    if (data_) {
        for (auto& section: *data_) {
            sections.push_back(section.first);
        }
    }
}

void IniParser::GetSectionKeys(const string& section_name, vector<string>& keys) {
    keys.clear();
    ScopedReadLock lock(&data_lock_);
    if (data_) {
        auto section = data_->find(section_name);
        if (section != data_->end()) {
            for (auto& kv: section->second) {
                keys.push_back(kv.first);
            }
        }
    }
}

void IniParser::Stop() {
    if (check_thread_.IsRunning()) {
        check_thread_.Stop(true);
    }
}

void IniParser::CaseTransform(string& str) {
    if (!case_sensitive_) {
        std::transform(str.begin(), str.end(), str.begin(), ::tolower);
    }
}

ZUTIL_NAMESPACE_END