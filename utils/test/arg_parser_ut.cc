//
// Created by lizhizhou on 2016/1/25.
//

#include <gtest/gtest.h>
#include "arg_parser.h"
#include "log.h"

ZUTIL_NAMESPACE_USE;

void print_string(const string& k, const string& v) {
    DEBUG_LOG(k << "-" << v);
}

TEST(ArgParserTest, SimpleArgTest) {
    ArgParser arg_parser;

    EXPECT_TRUE(arg_parser.Add<string>("file", "input file", __FILE__, 'f', false, print_string));
    EXPECT_TRUE(arg_parser.Add<int>("count", "input count", 0, 'c', true));

    vector<string> argv;
    argv.push_back("ut");
    argv.push_back("--file=ut.cc");
    argv.push_back("--help");
    // this will fail because required arg count is not here
    EXPECT_FALSE(arg_parser.CheckAndParse(argv));

    // it's ok now
    argv.push_back("-c");
    argv.push_back("1");
    EXPECT_TRUE(arg_parser.CheckAndParse(argv));

    int32_t count = 0;
    EXPECT_TRUE(arg_parser.Get("count", count));
    EXPECT_EQ(1, count);
}

TEST(ArgParserTest, RangeArgTest) {
    ArgParser arg_parser;
    EXPECT_TRUE(arg_parser.Add<int>("count1", "input count", 1, 'c', false, nullptr, make_range(0, 3)));
    EXPECT_TRUE(arg_parser.Add<int>("count2", "input count", 1, 'd', false, nullptr, make_range(0, 3)));
    vector<string> argv;
    argv.push_back("ut");
    argv.push_back("--count1=1");
    EXPECT_TRUE(arg_parser.CheckAndParse(argv));
    argv.push_back("--count2=10");
    EXPECT_FALSE(arg_parser.CheckAndParse(argv));
}

TEST(ArgParserTest, OptionArgTest) {
    ArgParser arg_parser;
    EXPECT_TRUE(arg_parser.Add<int>("count1", "input count", 1, 'c', false, nullptr, make_range(0, 3)));
    vector<string> schemes = {"http", "https"};
    EXPECT_TRUE(arg_parser.Add<string>("scheme1", "url scheme", "http", 's', false, nullptr, make_options(schemes)));
    EXPECT_TRUE(arg_parser.Add<string>("scheme2", "url scheme", "http", 'S', false, nullptr, make_options(schemes)));
    vector<string> argv;
    argv.push_back("ut");
    argv.push_back("--scheme1=http");
    EXPECT_TRUE(arg_parser.CheckAndParse(argv));
    argv.push_back("--scheme2=htaas");
    EXPECT_FALSE(arg_parser.CheckAndParse(argv));
}