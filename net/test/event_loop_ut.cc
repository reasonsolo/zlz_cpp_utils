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

void print_goodbye() {
    printf("goodbye timer\n");
}

void stop_loop(EventLoop* loop) {
    printf("stopping loop\n");
    if (loop)
        loop->Stop();
}

TEST(EventLoopTest, TimerTest) {
    EventLoop* loop = new EventLoop();
    int64_t time = TimeUtils::GetTickMS();
    loop->RunAfter(time - 10000, std::function<void()>(&print_hello));
    loop->RunAfter(time + 1000, std::function<void()>(&print_hello));
    loop->RunAfter(time + 1500, std::function<void()>(&print_goodbye));
    std::function<void()> func = std::bind(stop_loop, nullptr);
    loop->RunAfter(time + 2000, func);
    loop->RunAfter(time + 3000, std::bind(stop_loop, loop));

    loop->Start();
}
