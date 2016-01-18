//
// Created by lizhizhou on 2016/1/18.
//

#ifndef ZUTILS_SYNC_LOGGER_H
#define ZUTILS_SYNC_LOGGER_H

#include "logger.h"
#include "lock.h"

ZUTIL_NAMESPACE_BEGIN

class SyncLogger: public Logger {
public:
    SyncLogger(const string& name, uint32_t rotation_size);
    ~SyncLogger();

protected:
    pthread_mutex_t lock_;
};

ZUTIL_NAMESPACE_END

#endif //ZUTILS_SYNC_LOGGER_H
