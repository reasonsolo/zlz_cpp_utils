
//
// Created by zlz on 2016/1/11.
//

#ifndef ZUTILS_FILE_UTILS_H
#define ZUTILS_FILE_UTILS_H

#include "common.h"

ZUTIL_NAMESPACE_BEGIN

class FileUtils {
public:
    /*
     * return file size copied
     */
    static int64_t Copy(const string& src, const string& dst);

    /*
     * return true if renaming succeeded
     */
    static bool Rename(const string& from, const string& to);

    /*
     * return true if removing succeeded
     */
    static bool Remove(const string& filename);

};

ZUTIL_NAMESPACE_END


#endif //ZUTILS_FILE_UTILS_H
