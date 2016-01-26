//
// Created by zlz on 2016/1/11.
//

#ifndef ZUTILS_TIME_H
#define ZUTILS_TIME_H

#include "common.h"
#include <stdio.h>
#include <sys/time.h>
#include <unistd.h>

ZUTIL_NAMESPACE_BEGIN

class TimeUtils {

public:
    /*
     * a real-world time stamp
     */
    static int64_t GetTimestampInMS() {
        timeval tv {0, 0};
        gettimeofday(&tv, NULL);
        return static_cast<int64_t>(tv.tv_sec) * 1000 + tv.tv_usec / 1000;
    }

    /*
     * return a human-readable string representing the input timestamp (in 10^-3 second)
     */
    static string TimestampToString(int64_t ts_in_ms) {
        struct tm ts_struct;
        char buf[30] = { 0 };
        time_t ts = static_cast<time_t>(ts_in_ms / 1000);
        ts_struct = *localtime(&ts);
        strftime(buf, sizeof(buf), "%Y-%m-%d %X", &ts_struct);
        return string(buf);
    }


    /*
     * some dirty hacks
     */
    static double getCpuMhz() {
        FILE *fp = fopen("/proc/cpuinfo", "r");
        if (!fp)
            return 1;
        char buf[4096] = { 0 };
        fread(buf, 1, sizeof(buf), fp);
        fclose(fp);

        char *lp = strstr(buf, "cpu MHz");
        if (!lp)
            return 1;
        lp += strlen("cpu MHz");
        while (*lp == ' ' || *lp == '\t' || *lp == ':') {
            ++lp;
        }
        return atof(lp);
    }

    static uint64_t getCpuKhz() {
        return static_cast<uint64_t>(getCpuMhz() * 1000);
    }

    /*
     * get cpu cycle time in ms, relative time but not timestamp
     * do not expose this value to user
     */
    static uint64_t GetTickMS() {
        static uint64_t khz = getCpuKhz();
        return Counter() / khz;
    }

    /*
     * get cpu cycle time in us, relative time but not timestamp
     * * do not expose this value to user
     */
    static uint64_t GetTickUS() {
        static double mhz = getCpuMhz();
        return static_cast<uint64_t>(Counter() / mhz);
    }

    /*
     * get cpu cycle counter
     */
    static uint64_t Counter(void) {
        register uint32_t lo, hi;
        register unsigned long long o;
        __asm__ __volatile__ (
        "rdtscp" : "=a"(lo), "=d"(hi)
        );
        o = hi;
        o <<= 32;
        return (o | lo);
    }
};

ZUTIL_NAMESPACE_END

#endif //ZUTILS_TIME_H
