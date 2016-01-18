//
// Created by lizhizhou on 2016/1/18.
//

#include "file_utils.h"
#include "zfile.h"

ZUTIL_NAMESPACE_BEGIN

int64_t FileUtils::Copy(const string& src, const string& dst) {

    File fsrc(src);
    File fdst(dst);
    if (!fsrc.SetRead().Open() || fdst.SetWrite().SetCreate().SetExcl().Open()) {
        handle_sys_error("cannot open file to copy:");
        return -1;
    }

    // use smaller buffer to save memory while copying big files
    char buf[kIOBufMax];
    int64_t file_size = 0;
    for (; ;) {
        // stupid spelling
        int64_t readed = fsrc.Read(buf, sizeof(buf) - 1);
        if (readed > 0) {
            for (; ;) {
                if (fdst.Write(buf, static_cast<uint32_t>(readed)) != readed) {
                    return -1;
                }
                break;
            }
            file_size += readed;
        } else if (readed == 0) {
            break;
        } else {
            return -1;
        }
    }

    return file_size;
}

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

ZUTIL_NAMESPACE_END
