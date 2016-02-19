
//
// Created by lizhizhou on 2016/2/16.
//

#include "ev_thread.h"
#include "log.h"

ZUTIL_EV_NAMESPACE_BEGIN

EvThread::EvThread(): base_(nullptr), event_(nullptr), thread_(nullptr) {

}

EvThread::~EvThread() {
    Clearup();
}

void EvThread::Start() {
    thread_->Start();
}

void EvThread::Stop() {
    Defer(std::bind(event_base_loopbreak, base_));
}

void EvThread::Run() {
    if (base_) {
        event_base_loop(base_, 0);
    } else {
        FATAL_LOG(ToString() << " is not initialized");
    }
}

void EvThread::Init() {
    if (!base_) {
        sensor_ = new Sensor;
        sensor_->SetNonBlocking();
        base_ = event_base_new();
        event_ = event_new(base_, sensor_->read_fd(), EV_READ | EV_PERSIST,
                           (event_callback_fn) EvThread::HandleCmd, this);
        event_add(event_, nullptr);
        thread_ = new Thread(std::bind(&EvThread::Run, this));
    }
}

void EvThread::Clearup() {
    if (event_) {
        event_free(event_);
    }
    if (base_) {
        event_base_free(base_);
    }
    delete thread_;
}

void EvThread::HandleCmd(evutil_socket_t fd, int16_t which, void* arg) {
    EvThread* self;
    self = (EvThread*) arg;

    EvCallback cb;
    if (self->sensor_->Receive(&cb, sizeof(cb)) == sizeof(cb)) {
        if (cb) {
            DEBUG_LOG(self->ToString() << " get a cb message");
            cb();
        }
    }
}

void EvThread::Defer(EvCallback cb) {
    if (sensor_->Send(static_cast<void*>(&cb), sizeof(EvCallback)) == sizeof(EvCallback)) {
        ERROR_LOG(ToString() << " cannot send to sensor, errno " << errno);
    }
}

ZUTIL_EV_NAMESPACE_END