//
// Created by lizhizhou on 2016/1/14.
//

#include <gtest/gtest.h>
#include "zthread.h"
#include "atomic.h"

ZUTIL_NAMESPACE_USE;

AtomicInt<int32_t> count(0);
const int32_t kCountLimit = 20;

class WatcherThread: public Thread {
public:
    virtual void Run() {
        while (count.Get() < kCountLimit) {
            DEBUG_LOG("watcher is going to sleep " << count.Get());
            Sleep();
            DEBUG_LOG("watcher is waken up " << count.Get());
            count.AddAndGet(kCountLimit);
        }
        DEBUG_LOG("watcher will exit");
    }
};

class IncrThread: public Thread {
public:
    void SetWatcher(Thread* watcher) {
        watcher_ = watcher;
    }
    virtual void Run() {
        for (int32_t i = 0; i < kCountLimit; i++) {
            if (count.Get() == kCountLimit) {
                watcher_->Wakeup();
            }
            DEBUG_LOG("incr count " << count.Incr());
            Sleep(10);
        }
    }
private:
    Thread* watcher_;
};


TEST(ThreadTest, CondTest) {
    vector<Thread*> threads;
    Thread* watcher = new WatcherThread();
    watcher->Start();
    for (int32_t i = 0; i < 3; i++) {
        IncrThread* incr = new IncrThread();
        incr->SetWatcher(watcher);
        threads.push_back(incr);
        incr->Start();
        DEBUG_LOG("start incr " << incr->thread_id());
    }
    watcher->Join();
    for (auto t: threads) {
        t->Join();
    }
}

void print_sth(const string& sth, int32_t* count) {
    DEBUG_LOG("print " << sth);
    *count = 10;
}

TEST(ThreadTest, FunctorTest) {
    int32_t count = 0;
    Thread* thread = new Thread(std::bind(&print_sth, string("sth"), &count));
    thread->Start();
    thread->Stop(true);
    EXPECT_EQ(10, count);

}

