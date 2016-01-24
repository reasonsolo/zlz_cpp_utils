//
// Created by zlz on 2016/1/11.
//

#ifndef ZUTILS_ARGS_PARSER_H
#define ZUTILS_ARGS_PARSER_H

#include "common.h"
#include "container_utils.h"

ZUTIL_NAMESPACE_BEGIN

class Argument;

struct SimpleField;
struct RangeField;
struct OptionalField;

class ArgParser {
public:
    ArgParser();

    ~ArgParser();

    template<typename T, typename F>
    void Add(const string& key, const string& desc = "",
             const char abbev = 0, bool is_required = true,
             T def = T(), F field = F()) {
        if (args_.find(key) == args_.end()) {
            ArgumentBase* arg = new ArgumentWithField<T, F>(key, desc, abbev, is_required, def, field);
            ordered_args_.push_back(arg);
            args_[key] = arg;
        }
    }

    template <typename T>
    bool Get(const string& key, T& val) const {
        auto it = args_.find(key);
        if (it != args_.end()) {
            ArgumentWithValue* arg = dynamic_cast<ArgumentWithValue*>(it->second);
            return arg->GetValue(val);
        }
        return false;
    }

    bool Parse(int32_t argc, char* argv[]) {
        return false;
    }


private:
    string prog_name_;
    string description_;
    string prefix_char_;
    string input_;
    map<string, string> raw_kv_pair_;
    map<string, ArgumentBase*> args_;
    vector<ArgumentBase*> ordered_args_;
};

template<typename T>
struct SimpleField {
    bool operator()(const string& str, T& val) {
        stringstream ss;
        if (ss << str && ss >> val && ss.eof()) {
            return true;
        }
        return false;
    }
};

template<typename T>
struct RangeField {
    RangeField(const T& low, const T& up) : lower(low), upper(up) {

    }

    bool operator()(const string& str, T& val) {
        T t;
        SimpleField<T> sf;
        if (sf(str, t) && lower <= t && t <= upper) {
            val = t;
            return true;
        }
        return false;
    }

    T lower;
    T upper;
};

template<typename T>
RangeField<T> make_range(const T& low, const T& up) {
    return RangeField<T>(low, up);
}

template<typename T>
struct OptionField {
    OptionField(const vector<T>& options) : options_(options) {

    }

    bool operator()(const string& str, T& val) {
        T t;
        SimpleField<T> sf;
        if (sf(str, t) && ContainerUtils::Contains(options_, t)) {
            val = t;
            return true;
        }
        return false;
    }

    vector<T> options_;
};

// an interface for templated class Argument
class ArgumentBase {
    virtual bool is_set() const = 0;

    virtual bool is_required() const = 0;

    virtual bool is_valid() const = 0;

    virtual string default_str() const = 0;

    virtual string key() const = 0;

    virtual string desc() const = 0;

    virtual char abbev() const = 0;

};

template<typename T>
class ArgumentWithValue: public ArgumentBase {
public:

    virtual bool GetValue(T& val) const = 0;

    virtual bool Parse(string& str) = 0;
};

template<typename T, typename F>
class ArgumentWithField : public ArgumentWithValue {
public:
    ArgumentWithField(const string& key, const string& desc,
             const char abbev = '\0', bool is_required, const T& defaultv = T(), F field = F()) :
            key_(key), desc_(desc), abbev_(abbev), is_set_(false), is_required_(is_required),
            default_(defaultv), field_(field) {

    };

    virtual ~ArgumentWithField() {

    }

    virtual void set_value(const string& val) {
        is_set_ = true;
        str_value_ = val;
    }

    bool is_set() const {
        return false;
    }

    bool is_required() const {
        return is_required_;
    }

    bool is_valid() const {
        return is_required() ? is_set() : true;
    }

    string default_str() const {
        return StringUtils::ToString(default_);
    }

    string key() const {
        return key_;
    }

    string desc() const {
        return desc_;
    }

    char abbev() const {
        return abbev_;
    }

protected:
    bool ReadValue(const string& str_value) {
        if (field_(str_value, value_)) {
            is_set_ = true;
            return true;
        }
        return false;
    }

    string key_;
    string str_value_;
    string desc_;
    char abbev_;
    bool is_set_;
    bool is_required_;
    T value_;
    T default_;
    F field_;
};


ZUTIL_NAMESPACE_END


#endif //ZUTILS_ARGS_PARSER_H
