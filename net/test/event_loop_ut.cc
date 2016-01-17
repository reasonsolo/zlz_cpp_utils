//
// Created by zlz on 2016/1/14.
//


#include <gtest/gtest.h>
#include "event_loop.h"
#include "channel.h"
#include "time_utils.h"

ZUTIL_NAMESPACE_USE;
ZUTIL_NET_NAMESPACE_USE;

void print_hello() {
    printf("hello timer\n");
}

void stop_loop(EventLoop* loop) {
    loop->Stop();
}

TEST(EventLoopTest, TimerTest) {
    EventLoop* loop = new EventLoop();
    int64_t time = TimeUtils::GetTickMS() + 5000;
    loop->RunAfter(time, std::function<void()>(&print_hello));
    loop->RunAfter(time, std::function<void()>(&print_hello));
    loop->RunAfter(time + 1000, std::bind(&stop_loop, loop));
    loop->Start();
}
