//
// Created by lizhizhou on 2016/1/18.
//

#ifndef ZUTILS_SYNC_LOGGER_H
#define ZUTILS_SYNC_LOGGER_H

#include "logger.h"
#include "lock.h"
#include <iostream>
#include <fstream>

ZUTIL_NAMESPACE_BEGIN

class SyncLogger : public Logger {
public:
    SyncLogger(const string& name, uint32_t rotation_size);

    ~SyncLogger();

    virtual void Prepare();

    virtual void ClearUp();

protected:
    virtual void DoLog(const LogRecord& record);

    void BeforeLog();

    void AfterLog();

    void Rotate();
    Lock lock_;
    // pthread_mutex_t lock_;
    std::ofstream fstream_;
};

ZUTIL_NAMESPACE_END

#endif //ZUTILS_SYNC_LOGGER_H
