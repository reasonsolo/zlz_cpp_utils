//
// Created by lizhizhou on 2016/2/17.
//

#include "ev_uri.h"
#include "string_utils.h"
#include "type_utils.h"
#include "log.h"

ZUTIL_EV_NAMESPACE_BEGIN

const map<string, EvUriScheme> schemes_map(schemes, schemes + ArraySize(schemes));

EvUri::EvUri() {

}

EvUri::EvUri(const string& full) : full_(full) {
    Parse(full_);
}

void EvUri::Parse(const string& full) {
    Clear();
    full_ = full;
    // start position for each phase
    string::size_type domain_pos = 0;
    string::size_type port_pos = 0;
    string::size_type path_pos = 0;
    string::size_type domain_end_pos = 0;

    // parse scheme
    // set http as default
    scheme_ = EvUriScheme::kHttp;
    string scheme_str;
    static string scheme_suffix = "://";
    string::size_type scheme_end = full.find(scheme_suffix);
    if (scheme_end != string::npos) {
        scheme_str = full.substr(0, scheme_end);
        if (scheme_str.size() > 0 && scheme_str.find("/") == string::npos) {
            auto scheme_it = schemes_map.find(scheme_str);
            if (scheme_it != schemes_map.end()) {
                scheme_ = scheme_it->second;
            } else {
                scheme_ = EvUriScheme::kOther;
            }
            domain_pos = scheme_end + scheme_suffix.size();
        }
    }

    // parse domain and port
    if ((domain_end_pos = full_.find("/", domain_pos)) != string::npos) {
        path_pos = domain_end_pos;
    } else {
        path_pos = full_.size();
    }
    // :port must be ahead of path
    switch (scheme_) {
        case EvUriScheme::kHttp:
            port_ = 80;
            break;
        case EvUriScheme::kHttps:
            port_ = 443;
            break;
        default:
            port_ = 0;
    }
    if ((domain_end_pos = full_.find(":", domain_pos + 1)) != string::npos && domain_end_pos < path_pos) {
        port_pos = domain_end_pos;
        string port_str = full_.substr(port_pos + 1, path_pos - port_pos - 1); // omit ':'
        AnyCast(port_str, port_);
    } else {
        port_pos = path_pos;
    }
    domain_ = full_.substr(domain_pos, port_pos - domain_pos);

    // path, query and fragment
    string::size_type query_pos = full_.find("?", domain_pos);
    string::size_type frag_pos = full_.find("#", domain_pos);
    path_ = full_.substr(path_pos, std::min({full.size(), query_pos, frag_pos}) - path_pos);
    if (query_pos != string::npos && query_pos < frag_pos) {
        query_pos++;
        query_ = full_.substr(query_pos, std::min(full_.size(), frag_pos) - query_pos);
    }
    ParseQuery(query_, query_map_);
    if (frag_pos != string::npos) {
        frag_pos++;
        fragment_ = full_.substr(frag_pos);
    }
}

bool EvUri::ParseQuery(const string& query, map<string, string>& query_map) {
    return StringUtils::Split(query, "&", "=", query_map) > 0;
}

void EvUri::Clear() {
    full_.clear();
    scheme_ = EvUriScheme::kHttp;
    domain_.clear();
    port_ = 80;
    path_.clear();
    query_.clear();
    fragment_.clear();
    query_map_.clear();
}

ZUTIL_EV_NAMESPACE_END