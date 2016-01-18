
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
     * TODO: implement this
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

    /*
     * split a path to directory name and filename
     */
    static void GetDirectoryAndFile(const string& path, string& dirname, string& filename);

    static string GetCurrentWorkPath();

    int64_t GetModificationTime(const string& filename);

    int64_t GetFileSize(const string& filename);

    int32_t GetFileMode(const string& filename);
};

ZUTIL_NAMESPACE_END


#endif //ZUTILS_FILE_UTILS_H
