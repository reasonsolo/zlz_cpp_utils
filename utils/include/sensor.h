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
NO_COPY_CLASS(Sensor) {
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

    virtual void Execute() = 0;

protected:
    void Notify() {
        static char signal = 'x';
        while (true) {
            if (write(fds_[1], &signal, sizeof(signal)) == -1) {
                if (EINTR == errno) {
                    continue;
                }
            }
            break;
        }
    }

    void GetSignal(int32_t signal) {
        char signals[1024];
        while (true) {
            if (read(fds_[0], reinterpret_cast<void*>(signals), static_cast<size_t>(signal))) {
                if (EINTR == errno) {
                    continue;
                }
            }
            break;
        }
    }

    int32_t fds_[2];

};

ZUTIL_NAMESPACE_END

#endif //ZUTILS_SENSOR_H
