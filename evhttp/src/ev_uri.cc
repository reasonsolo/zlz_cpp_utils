//
// Created by lizhizhou on 2016/2/17.
//

#include "ev_uri.h"
#include "string_utils.h"

ZUTIL_EV_NAMESPACE_BEGIN

const map<const char*, EvUriScheme> schemes_map(schemes, schemes + ArraySize(schemes));

EvUri::EvUri() {

}

EvUri::EvUri(const string& full): full_(full) {
    Parse(full_);
}

void EvUri::Parse(const string& full) {
    full_ = full;
}

bool EvUri::ParseQuery(const string& query, map<string, string>& query_map) {
    return StringUtils::Split(query, "&", "=", query_map) > 0;
}

size_t EvUri::ParseScheme(const string& input, size_t start_pos, EvUriScheme& scheme) {
    string scheme_raw;
    static string scheme_suffix = "://";
    string::size_type scheme_end = input.find(scheme_suffix, start_pos);
    if (scheme_end != string::npos) {
        scheme_raw = input.substr(0, scheme_end);
        if (scheme_raw.size() > 0 && scheme_raw.find("/") == string::npos) {
            auto scheme_it = schemes_map.find(scheme_raw.c_str());
            if (scheme_it != schemes_map.end()) {
                scheme = scheme_it->second;
            } else {
                scheme = EvUriScheme::kOther;
            }
            return start_pos + scheme_end + scheme_suffix.size();
        }
    }
    // use http by default
    scheme = EvUriScheme::kHttp;
    return start_pos;
}

size_t EvUri::ParseDomainPort(const string& input, size_t start_pos, string& domain, int32_t& port) {

}

size_t EvUri::ParsePathFile(const string& input, size_t start_pos, string& path, string& file);

size_t EvUri::ParseQuery(const string& input, size_t start_pos, string& query);

size_t EvUri::ParseFragment(const string& input, size_t start_pos, string& fragment);



ZUTIL_EV_NAMESPACE_END