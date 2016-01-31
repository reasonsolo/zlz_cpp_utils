//
// Created by zlz on 2016/1/31.
//

#ifndef ZUTILS_BUFFER_H
#define ZUTILS_BUFFER_H

#include "common.h"

ZUTIL_NET_NAMESPACE_BEGIN


/// A buffer class modeled after org.jboss.netty.buffer.ChannelBuffer
///
/// +-------------------+------------------+------------------+
/// | prependable bytes |  readable bytes  |  writable bytes  |
/// |                   |     (CONTENT)    |                  |
/// +-------------------+------------------+------------------+
/// |                   |                  |                  |
/// 0      <=      readerIndex   <=   writerIndex    <=     size

class Buffer {
public:
    static const size_t kInitSize = 1024;
    static const size_t kCheapPrepend = 8;

    explicit Buffer(size_t init_size = 1024) : buffer_(init_size + kCheapPrepend), read_index_(kCheapPrepend),
                                               write_index_(read_index_) {

    }

    size_t PrePendableSize() const {
        return read_index_;
    }

    size_t ReadableSize() const {
        return write_index_ - read_index_;
    }

    size_t WritableSize() const {
        return buffer_.size() - write_index_;
    }

    const char* Begin() const {
        return &(buffer_[0]);
    }

    char* Begin() {
        return &(buffer_[0]);
    }

    const char* Readable() const {
        return Begin() + read_index_;
    }

    const char* Writable() const {
        return Begin() + write_index_;
    }

    char* Readable() {
        return Begin() + read_index_;
    }

    char* Writable() {
        return Begin() + write_index_;
    }

    void Take(size_t size) {
        if (size < ReadableSize()) {
            read_index_ += size;
        } else {
            TakeAll();
        }
    }

    string TakeString(size_t size) {
        string ret;
        if (size < ReadableSize()) {
            ret.assign(Readable(), size);
            Take(size);
        } else {
            ret.assign(Readable(), ReadableSize());
            TakeAll();
        }
        return ret;
    }

    void Receive(size_t size) {
        assert(size <= WritableSize());
        write_index_ += size;
    }

    void TakeAll() {
        read_index_ = kCheapPrepend;
        write_index_ = kCheapPrepend;
    }

    string TakeAllString() {
        string ret(Readable(), ReadableSize());
        TakeAll();
        return ret;
    }

    void Reserve(size_t size) {
        if (size > WritableSize()) {
            buffer_.resize(write_index_ + size);
        } else {
            // shrink space
            size_t readable_size = ReadableSize();
            std::copy(Readable(), Writable(), Begin() + kCheapPrepend);
            read_index_ = kCheapPrepend;
            write_index_ = read_index_ + readable_size;
            assert(readable_size == ReadableSize());
        }
    }

    void Append(const char* data, size_t size) {
        if (size > WritableSize()) {
            Reserve(size);
        }
        std::copy(data, data + size, Writable());
        Receive(size);
    }

    int32_t ReadFromFd(int32_t fd, int32_t& error) {
        char ext_buf[kIOBufMax];
        struct iovec vec[2];
        vec[0].iov_base = Writable();
        vec[0].iov_len  = WritableSize();
        vec[1].iov_base = ext_buf;
        vec[1].iov_len  = sizeof(ext_buf);
        ssize_t n = readv(fd, vec, 2);
        if (n < 0) {
            error =  errno;
        } else if (n  <= WritableSize()) {
            write_index_ += n;
        } else {
            write_index_ = buffer_.size();
            Append(ext_buf, n - WritableSize());
        }
    }

private:

    vector<char> buffer_;
    size_t read_index_;
    size_t write_index_;
};


ZUTIL_NET_NAMESPACE_END

#endif //ZUTILS_BUFFER_H
