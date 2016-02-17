//
// Created by zlz on 2016/1/19.
//

#include "sys_utils.h"

ZUTIL_NAMESPACE_BEGIN

string SysUtils::GetProcFullName() {
    return program_invocation_name;
}

string SysUtils::GetProcShortName() {
    return program_invocation_short_name;
}

string SysUtils::ReplaceSuffix(const string& filename, const string& suffix) {
    // FIXME: find . and replace suffix
    return filename + "." + suffix;
}

ZUTIL_NAMESPACE_END