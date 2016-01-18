//
// Created by lizhizhou on 2016/1/18.
//

#include "file_utils.h"
#include <fcntl.h>

ZUTIL_NAMESPACE_BEGIN

bool FileUtils::Rename(const string& from, const string& to) {
    if (rename(from.c_str(), to.c_str()) == -1) {
        handle_sys_error("cannot rename file:");
        return false;
    }
    return true;
}

bool FileUtils::Remove(const string& filename) {
    if (remove(filename.c_str()) == -1) {
        handle_sys_error("cannot remove file:");
        return false;
    }
    return true;
}

void FileUtils::GetDirectoryAndFile(const string& path, string& dirname, string& filename) {
    string::size_type slash_pos = path.find_last_of("/");
    if (slash_pos == string::npos) {
        filename = path;
        dirname = "";
    } else {
        dirname = path.substr(0, slash_pos);
        filename = path.substr(slash_pos + 1, path.size() - slash_pos - 1);
    }
}

string FileUtils::GetCurrentWorkPath() {
    return "";
}

int64_t FileUtils::GetModificationTime(const string& filename) {
    int64_t time = -1;
    struct stat file_stat;
    if (stat(filename.c_str(), &file_stat) == 0) {
        time = file_stat.st_mtim.tv_sec * 1000 + file_stat.st_mtim.tv_sec % 1000000;
    }
    return time;
}

int64_t FileUtils::GetFileSize(const string& filename) {
    int64_t size = -1;
    struct stat file_stat;
    if (stat(filename.c_str(), &file_stat) == 0) {
        size = file_stat.st_size;
    }
    return size;
}

int32_t FileUtils::GetFileMode(const string& filename) {
    struct stat file_stat;
    if (stat(filename.c_str(), &file_stat)) {
        return file_stat.st_mode;
    }
    return -1;
}

ZUTIL_NAMESPACE_END
