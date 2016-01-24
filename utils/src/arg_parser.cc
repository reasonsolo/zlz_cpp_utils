//
// Created by zlz on 2016/1/25.
//

#include "arg_parser.h"
#include "log.h"

ZUTIL_NAMESPACE_BEGIN

template<typename T, typename F>
void ArgParser::Add(const string& key, const string& desc = "",
         const char abbev = 0, bool is_required = true,
         T def = T(), F field = F()) {
    if (args_.find(key) == args_.end()) {
        ArgumentBase* arg = new ArgumentWithField<T, F>(key, desc, abbev, is_required, def, field);
        ordered_args_.push_back(arg);
        args_[key] = arg;
    } else {
        WARN_LOG(ToString() << " cannot add duplicate arg key " << key);
    }
}

template <typename T>
bool ArgParser::Get(const string& key, T& val) const {
    auto it = args_.find(key);
    if (it != args_.end()) {
        ArgumentWithValue* arg = dynamic_cast<ArgumentWithValue*>(it->second);
        return arg->GetValue(val);
    } else {
        WARN_LOG(ToString() << " cannot get arg key " << key);
    }
    return false;
}

bool ArgParser::Parse(int32_t argc, const char* const argv[]) {
    if (argc < 1) {
        return false;
    }
    if (prog_name_.empty()){
        prog_name_ = argv[0];
    }

    /*
     * get an abbev->key map
     */
    map<char, string> abbev_key_map;
    for (auto& kv: args_) {
        char abbrev = kv.second->abbrev();
        assert(kv.first == kv.second->key());
        if (abbev_key_map.find(abbrev) != abbev_key_map.end()) {
            ERROR_LOG(ToString() << " find ambiguous abbreviation " << abbrev
                      << " for " << kv.first);
            return false;
        }
        abbev_key_map[abbrev] = kv.second->key();
    }

    for (int32_t i = 1; i < argc; i++) {
        if (strncmp(argv[i], "--", 2) == 0) {
            string k, v;
            if (!SplitByEq(string(argv[i] + 2), k, v) || !SetKeyValue(k, v)) {
                ERROR_LOG(ToString() << " cannot parse argv " << argv[i]);

                return false;
            }
        } else if (strncmp(argv[i], "-", 1) == 0) {

        }
    }

    return false;
}

ZUTIL_NAMESPACE_END