//
// Created by zlz on 2016/1/11.
//

#ifndef ZUTILS_FILE_H
#define ZUTILS_FILE_H

#include "common.h"
#include <fcntl.h>

ZUTIL_NAMESPACE_BEGIN

NO_COPY_CLASS(File) {
public:
    File(const string& filename);

    virtual bool Open(int32_t flags = -1);

    virtual void Close();

    virtual ~File();

    string filename() const {
        return filename_;
    };

    int32_t fd() const {
        return fd_;
    };

    File& SetRead() {
        flags_ |= O_RDONLY;
        return *this;
    }

    File& SetWrite() {
        flags_ |= O_WRONLY | O_CREAT;
        return *this;
    }

    File& SetReadWrite() {
        flags_ |= O_RDWR | O_CREAT;
        return *this;
    }

    File& SetTrancate() {
        flags_ |= O_TRUNC;
        return *this;
    }

    File& SetAppend() {
        flags_ |= O_APPEND;
        return *this;
    }

    File& SetExcl() {
        flags_ |= O_EXCL;
        return *this;
    }

    File& SetCreate() {
        flags_ |= O_CREAT;
        return *this;
    }

    bool IsReadable() const;

    bool IsWritable() const;

    int64_t Write(const char* buf, uint32_t size);

    int64_t Read(char* buf, uint32_t size);

    int64_t ReadAll(string& str_buf);

    /*
     * this function will block until all data has been flushed
     */
    void Flush();

    int64_t GetSize();

    int64_t GetCreationTime();

    int64_t GetModificationTime();

    int32_t GetMode();

protected:

    string filename_;
    int32_t fd_;
    int32_t flags_;
};



ZUTIL_NAMESPACE_END

#endif //ZUTILS_FILE_H
