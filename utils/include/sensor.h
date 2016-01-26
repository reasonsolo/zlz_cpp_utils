//
// Created by lizhizhou on 2016/1/14.
//

#ifndef ZUTILS_SENSOR_H
#define ZUTILS_SENSOR_H

#include "common.h"

ZUTIL_NAMESPACE_BEGIN

/*
 * a pipe as a notification interface
 */
class Sensor {
public:
    Sensor() {
        assert(pipe(fds_) != -1);
    }

    virtual ~Sensor() {
        if (fds_[0] != -1) {
            close(fds_[0]);
            close(fds_[1]);
        }
    }

    int32_t read_fd() const {
        return fds_[0];
    }

    int32_t write_fd() const {
        return fds_[1];
    }

    void Notify() {
        static char finger = 'x';
        while (true) {
            if (write(fds_[1], &finger, sizeof(finger)) == -1) {
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
            if ((readed = read(fds_[0], reinterpret_cast<void*>(signals), signal))) {
                if (EINTR == errno) {
                    continue;
                }
                signal -= readed;
            }
        }
    }
private:

    int32_t fds_[2];

};

ZUTIL_NAMESPACE_END

#endif //ZUTILS_SENSOR_H
