//
// Created by lizhizhou on 2016/1/15.
//

#include <gtest/gtest.h>
#include "zthread.h"
#include "sensor.h"
#include "log.h"

//ZUTILS_NAMESPACE_USE;
using namespace zutil;

class TouchThread: public Thread {
public:
    void SetSensor(Sensor* sensor) {
        sensor_ = sensor;
    }
    void Run() {
        for (int32_t i = 0; i < 10; i++) {
            sensor_->Notify();
            DEBUG_LOG("notify " << i);
        }
    }
    Sensor* sensor_;
};

class FeelThread: public Thread {
public:
    void SetSensor(Sensor* sensor) {
        sensor_ = sensor;
    }
    void Run() {
        DEBUG_LOG("try to get 10 signal");
        sensor_->GetSignal(10);
        DEBUG_LOG("get 10 signal");
    }
    Sensor* sensor_;
};

TEST(SensorTest, Test) {
    Sensor* sensor = new Sensor;
    FeelThread* f = new FeelThread;
    TouchThread* t = new TouchThread;
    f->SetSensor(sensor);
    t->SetSensor(sensor);
    f->Start();
    t->Start();
    f->Join();
    t->Join();
}
