//
// Created by lizhizhou on 2016/1/18.
//

#include "zfile.h"
#include <sys/stat.h>
#include <fcntl.h>
#include "string_utils.h"

ZUTIL_NAMESPACE_BEGIN

File::File(const string& filename) : filename_(filename),
                                     fd_(-1) {

}

File::~File() {
    Close();
}


void File::Close() {
    if (fd_ >= 0) {
        close(fd_);
    }
}

bool File::Open(int32_t flags) {
    Close();
    if (flags >= 0) {
        flags_ = flags;
    }
    DEBUG_LOG("try open file " << filename_ << " flags " << flags_);
    fd_ = open(filename_.c_str(), flags_);
    if (fd_ == -1) {
        handle_sys_error("cannot open file");
    }
    return fd_ != -1;
}

bool File::IsReadable() const {
    return fd_ != -1 && (flags_ == O_RDONLY || (flags_ & O_RDWR) == O_RDWR);
}

bool File::IsWritable() const {
    return fd_ != -1 && ((flags_ & O_WRONLY) == O_WRONLY || (flags_ & O_RDWR) == O_RDWR);
}

int64_t File::Write(const char* buf, uint32_t size) {
    int64_t ret = -1;
    if (IsWritable()) {
        for (; ;) {
            ret = write(fd_, buf, size);
            if (ret != size) {
                if (errno == EINTR) {
                    continue;
                } else {
                    handle_sys_error("cannot write file: ");
                    return -1;
                }
            }
            break;
        }
    }
    return ret;
}

int64_t File::Read(char* buf, uint32_t size) {
    int64_t ret = -1;
    if (IsReadable()) {
        ret = read(fd_, buf, size);
        if (ret < 0) {
            handle_sys_error("cannot read file:");
        }
        DEBUG_LOG("read bytes " << ret);
    }

    return ret;
}

int64_t File::ReadAll(string& str_buf) {
    int64_t size = GetSize();
    DEBUG_LOG("try to read all get size " << size);
    if (size > 0) {
        str_buf.clear();
        str_buf.resize(size);
        return Read(StringUtils::AsArray(str_buf), size);
    }
    return 0;
}

void File::Flush() {
    // TODO: fix it
    if (fd_ != -1) {
        fsync(fd_);
    }
}

int64_t File::GetSize() {
    int64_t size = -1;
    struct stat file_stat;
    if (stat(filename_.c_str(), &file_stat) == 0) {
        size = file_stat.st_size;
    }
    return size;
}

int64_t File::GetModificationTime() {
    int64_t time = -1;
    struct stat file_stat;
    if (stat(filename_.c_str(), &file_stat) == 0) {
        time = file_stat.st_mtim.tv_sec * 1000 + file_stat.st_mtim.tv_sec % 1000000;
    }
    return time;
}

int32_t File::GetMode() {
    struct stat file_stat;
    if (stat(filename_.c_str(), &file_stat)) {
        return file_stat.st_mode;
    }
    return -1;
}


ZUTIL_NAMESPACE_END