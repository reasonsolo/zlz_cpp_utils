//
// Created by zlz on 2016/1/11.
//
// inspired by tanakh/cmdline: https://github.com/tanakh/cmdline

#ifndef ZUTILS_ARGS_PARSER_H
#define ZUTILS_ARGS_PARSER_H

#include "common.h"
#include "string_utils.h"
#include "sys_utils.h"
#include "type_utils.h"


ZUTIL_NAMESPACE_BEGIN

/*
 * a lexical cast with stringstream
 */
template<typename T>
struct SimpleField {
    bool set(const string& str, T& val, string& errmsg) {
        stringstream ss;
        if (ss << str && ss >> val && ss.eof()) {
            errmsg = StringUtils::ToString("cannot accept value ", val, " as ", ToString());
            return true;
        }
        return false;
    }

    string ToString() const {
        return get_typename(t);
    }

    T t;
};

template<typename T>
struct RangeField {
    RangeField(const T& low, const T& up) : lower(low), upper(up) {

    }

    bool set(const string& str, T& val, string& errmsg) {
        T t;
        SimpleField<T> sf;
        if (!sf.set(str, t, errmsg)) {
            return false;
        }
        if (lower <= t && t <= upper) {
            val = t;
            return true;
        }
        errmsg = StringUtils::ToString(t, " is not in range ", ToString());
        return false;
    }

    string ToString() const {
        stringstream ss;
        ss << "[" << lower << "," << upper << "]";
        return ss.str();
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
    OptionField(const vector<T>& opts) {
        for (auto& op: opts) {
            options.insert(op);
        }
    }

    bool set(const string& str, T& val, string& errmsg) {
        T t;
        SimpleField<T> sf;
        if (!sf.set(str, t, errmsg)) {
            return false;
        }
        if (options.find(t) != options.end()) {
            val = t;
            return true;
        }
        errmsg = StringUtils::ToString(t, " matches no options ", ToString());
        return false;
    }

    string ToString() const {
        stringstream ss;
        ss << "[";
        for (auto opt = options.begin(); opt != options.end(); opt++) {
            if (opt != options.begin()) {
                ss << "|";
            }
            ss << *opt;
        }
        ss << "]";
        return ss.str();
    }

    std::set<T> options;
};

template<typename T>
OptionField<T> make_options(vector<T> opts) {
    return OptionField<T>(opts);
}


// an interface for templated class Argument
class ArgumentBase {
public:
    ArgumentBase(const string& key, const string& desc,
                 const char abbrev = '\0', bool is_required = true) :
            key_(key), desc_(desc), abbrev_(abbrev), is_required_(is_required), is_set_(false) {

    }

    virtual bool is_set() const {
        return is_set_;
    }

    virtual bool set(const string& str_val, string& errmsg) {
        return true;
    }

    virtual bool is_required() const {
        return is_required_;
    }

    virtual bool is_valid() const {
        return !is_required_ || is_set_;
    }

    virtual string default_str() const {
        return "";
    }

    virtual bool need_value() const {
        return false;
    }

    virtual string key() const {
        return key_;
    }

    virtual const string& desc() const {
        return desc_;
    }

    virtual char abbrev() const {
        return abbrev_;
    }

    virtual string ToString() const {
        return "Arg:" + key_;
    }

    virtual string ToBasicString() const {
        stringstream ss;
        ss << "--" << key_;
        if (abbrev_ > 0) {
            ss << "|" << "-" << abbrev_;
        }
        return ss.str();
    }

    virtual string ToShortString() const {
        stringstream ss;
        ss << (is_required_ ? "<" : "[")
        << ToBasicString()
        << (is_required_ ? ">" : "]");
        return ss.str();
    }

    virtual string ToFullString() const {
        stringstream ss;
        ss << (is_required_ ? "required" : "optional") << ": " << ToBasicString();
        ss << "\t" << desc_;
        return ss.str();
    }

protected:
    string key_;
    string desc_;
    char abbrev_;
    bool is_required_;
    bool is_set_;
};

template<typename T>
class ArgumentWithValue : public ArgumentBase {
public:
    ArgumentWithValue(const string& key, const string& desc, const T& def,
                      const char abbrev = '\0', bool is_required = true) :
            ArgumentBase(key, desc, abbrev, is_required), def_(def) {

    }

    virtual bool get(T& val) {
        /*
         * template inheritance class, use this-> to refer to the super class member
         */
        if (this->is_set_) {
            val = val_;
        }
        return this->is_set_;
    }

    virtual bool need_value() const {
        return true;
    }

    bool is_valid() const {
        return !this->is_required_ || this->is_set_;
    }

protected:

    T def_;
    T val_;
};

class ArgumentWithCallback : public ArgumentBase {
public:
    ArgumentWithCallback(const string& key, const string& desc,
                         const char abbrev = '\0', bool is_required = true,
                         const std::function<void()>& callback = nullptr) :
            ArgumentBase(key, desc, abbrev, is_required), callback_(callback) {

    }

    virtual bool need_value() const {
        return false;
    }

    virtual void set() const {
        if (callback_) {
            callback_();
        }
    }

protected:
    std::function<void()> callback_;
};

template<typename T, typename F>
class ArgumentWithField : public ArgumentWithValue<T> {
public:
    ArgumentWithField(const string& key, const string& desc, const T& def, const char abbrev = '\0',
                      bool is_required = true, F field = F(),
                      const std::function<void(string&, T&)>& callback = nullptr) :
            ArgumentWithValue<T>(key, desc, def, abbrev, is_required), field_(field), set_callback_(callback) {

    };

    virtual ~ArgumentWithField() {

    }

    virtual bool need_value() const {
        return true;
    }

    virtual bool set(const string& val, string& errmsg) {
        this->is_set_ = field_.set(val, this->val_, errmsg);
        if (this->is_set_ && set_callback_) {
            set_callback_(this->key_, this->val_);
        }
        return this->is_set_;
    }

    virtual string ToBasicString() const {
        stringstream ss;
        ss << "--" << this->key_ << "=";
        if (this->abbrev_ > 0) {
            ss << "|" << "-" << this->abbrev_ << " ";
        }
        ss << field_.ToString();
        return ss.str();
    }

    virtual string ToFullString() const {
        stringstream ss;
        ss << (this->is_required_ ? "required" : "optional") << ": " << "--" << this->key_;
        if (this->abbrev_ > 0) {
            ss << "|" << "-" << this->abbrev_;
        }
        ss << "\t" << this->desc_ << "\tdefault: " << this->def_;
        return ss.str();
    }

protected:
    F field_;
    std::function<void(string&, T&)> set_callback_;
};


class ArgParser {
public:
    ArgParser(const string& desc = "") : desc_(desc) {
        prog_name_ = SysUtils::GetProcShortName();
        assert(Add("help", "print usage message", 'h', false, std::bind(PrintUsage, this)));
    }

    ~ArgParser() {
        for (auto& kv: args_) {
            delete kv.second;
        }
    }

    template<typename T, typename F>
    bool Add(const string& key, const string& desc, T def,
             const char abbrev = 0, bool is_required = true,
             const std::function<void(string&, T&)>& cb = nullptr, F field = F()) {
        if (args_.find(key) == args_.end()) {
            ArgumentBase* arg = new ArgumentWithField<T, F>(key, desc, def, abbrev, is_required, field, cb);
            ordered_args_.push_back(arg);
            args_[key] = arg;
            return true;
        }
        return false;
    }

    template<typename T>
    bool Add(const string& key, const string& desc, T def,
             const char abbrev = 0, bool is_required = true,
             const std::function<void(string&, T&)>& cb = nullptr) {
        return Add<T, SimpleField<T> >(key, desc, def, abbrev, is_required, cb, SimpleField<T>());
    }

    bool Add(const string& key, const string& desc = "",
             const char abbev = 0, bool is_required = true, const std::function<void()>& cb = nullptr) {
        if (args_.find(key) == args_.end()) {
            ArgumentBase* arg = new ArgumentWithCallback(key, desc, abbev, is_required, cb);
            ordered_args_.push_back(arg);
            args_[key] = arg;
            return true;
        }
        return false;
    }

    template<typename T>
    bool Get(const string& key, T& val) {
        ArgumentBase* arg = args_[key];
        if (arg && arg->is_set() && arg->need_value()) {
            return dynamic_cast<ArgumentWithValue<T>*>(arg)->get(val);
        }
        return false;
    }

    bool Get(const string& key) {
        ArgumentBase* arg = args_[key];
        if (arg) {
            return arg->is_set();
        }
        return false;
    }

    bool Parse(int32_t argc, const char* const argv[]) {
        if (argc < 1) {
            return false;
        }
        if (prog_name_.empty()) {
            prog_name_ = argv[0];
        }
        err_msgs_.clear();
        rest_args_.clear();
        input_args_.clear();

        /*
         * get an abbev->key map
         */
        map<char, string> abbrev_key_map;
        for (auto& kv: args_) {
            char abbrev = kv.second->abbrev();
            assert(kv.first == kv.second->key());
            if (abbrev_key_map.find(abbrev) != abbrev_key_map.end()) {
                return false;
            }
            abbrev_key_map[abbrev] = kv.second->key();
        }

        bool parse_success = true;
        for (int32_t i = 1; i < argc; i++) {
            // "--" format
            string k, v;
            if (strncmp(argv[i], "--", 2) == 0) {
                SplitByEq(string(argv[i] + 2), k, v);
                parse_success = SetKeyValue(k, v);
            } else if (strncmp(argv[i], "-", 1) == 0) {
                // "-" format
                if (argv[i][1] != 0) {
                    if (abbrev_key_map.find(argv[i][1]) != abbrev_key_map.end()) {
                        k = abbrev_key_map[argv[i][1]];
                        // get next argv as value, the value cannot startwith "-" or "--"
                        if (i + 1 < argc && argv[i + 1][0] != '-') {
                            v = argv[++i];
                        }
                        parse_success = SetKeyValue(k, v);
                    }
                }
            } else {
                rest_args_.push_back(argv[i]);
            }
        }

        return parse_success && Validate();
    }

    bool CheckAndParse(int32_t argc, const char* const argv[]) {
        if (!Parse(argc, argv)) {
            PrintUsage(this);
            PrintErrMsgs();
            return false;
        }
        return true;
    }

    bool CheckAndParse(vector<string>& argv) {
        vector<const char*> char_argv;
        for (auto& arg: argv) {
            char_argv.push_back(arg.c_str());
        }
        return CheckAndParse(static_cast<int32_t>(char_argv.size()), &char_argv[0]);
    }

    string Usage() const {
        stringstream ss;
        ss << "Usage: " << prog_name_ << " ";
        for (auto arg: ordered_args_) {
            ss << arg->ToShortString() << " ";
        }
        ss << "\n";
        ss << "\t" << desc_ << "\n";
        for (auto arg: ordered_args_) {
            ss << "\t" << arg->ToFullString() << "\n";
        }
        return ss.str();
    }

    string ToString() const {
        return "ArgParser-" + prog_name_;
    }

    bool Validate() {
        for (auto& it: args_) {
            if (!it.second->is_valid()) {
                if (it.second->is_required() && input_args_.find(it.first) == input_args_.end()) {
                    err_msgs_.push_back(StringUtils::ToString(it.first, " error: required but missing"));
                }
                return false;
            }
        }
        return true;
    }

    /*
     * args that start with neither "--" nor "-"
     */
    const vector<string> GetRestArgs() const {
        return rest_args_;
    }

private:
    void SplitByEq(const string& str, string& k, string& v) {
        string::size_type eq_pos = str.find_first_of('=');
        if (eq_pos != string::npos) {
            k = str.substr(0, eq_pos);
            v = str.substr(eq_pos + 1);
        } else {
            k = str;
        }
    }

    bool SetKeyValue(const string& k, const string& v) {
        ArgumentBase* arg = args_[k];
        input_args_[k] = v;
        bool success = true;
        if (arg) {
            if (arg->need_value()) {
                string err_msg;
                success = arg->set(v, err_msg);
                if (!success) {
                    err_msgs_.push_back(string(k).append(" error: ").append(err_msg));
                }
            } else {
                ArgumentWithCallback* arg_with_cb = dynamic_cast<ArgumentWithCallback*>(arg);
                arg_with_cb->set();
            }
        }
        return success;
    }

    static void PrintUsage(ArgParser* arg_parser) {
        std::cerr << arg_parser->Usage() << std::endl;
    }

    void PrintErrMsgs() {
        for (auto& msg: err_msgs_) {
            std::cerr << msg << std::endl;
        }
    }

    string prog_name_;
    string desc_;
    map<string, string> input_args_;
    map<string, ArgumentBase*> args_;
    vector<ArgumentBase*> ordered_args_;
    vector<string> rest_args_;
    vector<string> err_msgs_;
};

ZUTIL_NAMESPACE_END

#endif //ZUTILS_ARGS_PARSER_H
