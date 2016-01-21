//
// Created by zlz on 2016/1/13.
//

#include "string_utils.h"
#include "log.h"

ZUTIL_NAMESPACE_BEGIN

size_t StringUtils::Split(const string& source, const string& delim, vector<string>& tokens,
                          bool skip_empty) {
   DEBUG_LOG("split " << source);
    size_t count = 0;
    if (delim.empty()) {
        tokens.push_back(source);
    } else if (!source.empty()) {
        string::size_type last_delim = 0;
        string::size_type delim_pos = 0;
        while ((delim_pos = source.find(delim, last_delim)) != string::npos) {
            DEBUG_LOG("delim pos " << delim_pos << " last delim " << last_delim);
            if (last_delim != delim_pos || !skip_empty) {
                tokens.push_back(source.substr(last_delim, delim_pos - last_delim));
                DEBUG_LOG("add token " << source.substr(last_delim, delim_pos - last_delim));
                count++;
            }
            last_delim = delim_pos + delim.size();
        }
        if (last_delim < source.size()) {
            tokens.push_back(source.substr(last_delim));
            DEBUG_LOG("add token " << source.substr(last_delim));
            count++;
        }
    }
    return count;
}

size_t StringUtils::Split(const string& source, const string& delim, const string& val_delim,
                          map<string, string>& tokens) {
    size_t count = 0;
    vector<string> tmp_tokens;
    Split(source, delim, tmp_tokens);
    for (auto&& kv_pair: tmp_tokens) {
        string::size_type val_delim_pos = kv_pair.find(val_delim);
        if (val_delim_pos != string::npos) {
            tokens.insert(
                    make_pair(kv_pair.substr(0, val_delim_pos),
                              kv_pair.substr(val_delim_pos + val_delim.size())));
            DEBUG_LOG(kv_pair.substr(0, val_delim_pos) << ":" << kv_pair.substr(val_delim_pos + val_delim.size()));
            count++;
        }
    }
    return count;
}


ZUTIL_NAMESPACE_END