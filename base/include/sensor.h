//
// Created by lizhizhou on 2016/1/14.
//

#ifndef ZUTILS_SENSOR_H
#define ZUTILS_SENSOR_H

#include "common.h"
#include <sys/fcntl.h>

ZUTIL_NAMESPACE_BEGIN

enum SensorFd {
    kRead = 0,
    kWrite,
    kNum
};

/*
 * a pipe as a notification interface
 */
class Sensor {
public:
    Sensor() {
        assert(pipe(fds_) != -1);
    }

    virtual ~Sensor() {
        if (fds_[kRead] != -1) {
            close(fds_[0]);
            close(fds_[1]);
        }
    }

    int32_t read_fd() const {
        return fds_[kRead];
    }

    int32_t write_fd() const {
        return fds_[kWrite];
    }

    void SetNonBlocking() {
        is_nonblock_ = true;
        fcntl(fds_[kRead], F_SETFL, O_NONBLOCK);
        fcntl(fds_[kWrite], F_SETFL, O_NONBLOCK);
    }

    ssize_t Send(void* data, size_t len) {
        ssize_t n = 0;
        if (is_nonblock_) {
            // when writing n bytes (n < PIPE_BUF) to a nonblocking pipe,
            // write should succeeds immediately according to `man 7 pipe`
            // so better not write data of a large length
            while (n < len) {
                ssize_t written = write(fds_[kWrite], data + n, len - n);
                if (written == -1) {
                    if (errno == EAGAIN) {
                        continue;
                    }
                    if (errno == EINTR) {
                        break;
                    }
                }
                n += written;
            }
        } else {
            n = write(fds_[kWrite], data, len);
        }
        return n;
    }

    ssize_t Receive(void* data, size_t len) {
        return read(fds_[kRead], data, len);
    }

    void Notify() {
        static char finger = 'x';
        while (true) {
            if (write(fds_[kWrite], &finger, sizeof(finger)) == -1) {
                if (EINTR == errno) {
                    continue;
                }
            }
            break;
        }
    }

    void GetSignal(uint32_t signal) {
        char signals[1024];
        int32_t readed;
        while (signal > 0) {
            if ((readed = read(fds_[kRead], reinterpret_cast<void*>(signals), signal))) {
                if (EINTR == errno) {
                    continue;
                }
                signal -= readed;
            }
        }
    }
private:
    bool is_nonblock_;
    int32_t fds_[kNum];
};

ZUTIL_NAMESPACE_END

#endif //ZUTILS_SENSOR_H
