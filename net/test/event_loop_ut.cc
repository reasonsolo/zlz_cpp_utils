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
    loop->RunAt(std::function<void()>(&print_hello), time - 10000);
    loop->RunAt(std::function<void()>(&print_hello), time + 1000);
    loop->RunAt(std::function<void()>(&print_goodbye), time + 1500);
    std::function<void()> func = std::bind(stop_loop, nullptr);
    loop->RunAt(func, time + 2000);
    loop->RunAt(std::bind(stop_loop, loop), time + 3000);

    loop->Start();
}
