
//
// Created by lizhizhou on 2016/2/17.
//

#ifndef ZUTILS_EV_URI_H
#define ZUTILS_EV_URI_H

#include "common.h"

ZUTIL_EV_NAMESPACE_BEGIN

enum class EvUriScheme {
    kHttp,
    kHttps,
    kFtp,
    // TODO: more schemes
    kOther
};

static const pair<const char*, EvUriScheme> schemes[] = {
    make_pair("http", EvUriScheme::kHttp),
    make_pair("https", EvUriScheme::kHttps),
    make_pair("ftp", EvUriScheme::kFtp),
};

const map<const char*, EvUriScheme> schemes_map;

class EvUri {
public:

    EvUri();

    EvUri(const string& full);

    void Parse(const string& full);

    const string& full() const {
        return full_;
    }

    const EvUriScheme& scheme() const {
        return scheme_;
    }

    const string& domain() const {
        return domain_;
    }

    int32_t port() const {
        return port_;
    }

    const string& path() const {
        return path_;
    }

    const string& file() const {
        return file_;
    }

    const string& query() const {
        return query_;
    }

    const string& fragment() const {
        return fragment_;
    }

    const map<string, string>& query_map() const {
        return query_map_;
    }

    static string Encode(const string& input);

    static string Decode(const string& input);

private:
    static size_t ParseScheme(const string& input, size_t start_pos, EvUriScheme& scheme);

    static size_t ParseDomainPort(const string& input, size_t start_pos, string& domain, int32_t& port);

    static size_t ParsePathFile(const string& input, size_t start_pos, string& path, string& file);

    static size_t ParseQuery(const string& input, size_t start_pos, string& query);

    static size_t ParseFragment(const string& input, size_t start_pos, string& fragment);

    static bool ParseQuery(const string& query, map<string, string>& query_map);

    string full_; // eg: http://a:8080/b/c.html?d=1&e=2#fg
    EvUriScheme scheme_; // eg: http
    string domain_; // eg: a
    int32_t port_; // eg: 8080, 80 by default
    string path_; // eg: /b/
    string file_; // eg: c.html
    string query_; // eg: d=1&e=2
    string fragment_; // eg: fg
    map<string, string> query_map_; // d:1;e:2
    // TODO: authority

};

ZUTIL_EV_NAMESPACE_END

#endif //ZUTILS_EV_URI_H
