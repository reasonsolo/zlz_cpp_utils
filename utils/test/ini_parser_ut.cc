//
// Created by zlz on 2016/1/27.
//
#include <gtest/gtest.h>
#include "ini_parser.h"
#include <fstream>
#include "log.h"
#include "container_utils.h"

ZUTIL_NAMESPACE_USE;
using namespace std;

void ParseCb(IniParser* parser) {
    EXPECT_EQ(8080, parser->Get("server", "port", 111));
    EXPECT_STREQ("async", parser->Get<string>("log", "type", "tmp").c_str());
    EXPECT_STREQ("http", parser->Get<string>("server", "type", "def").c_str());
    EXPECT_STREQ("http", parser->Get<string>("server", "tYPe", "def").c_str());
    EXPECT_EQ(8080, parser->Get("server", "port", 111));
    EXPECT_EQ(111, parser->Get("server", "port1", 111));
    vector<string> names;
    parser->GetSections(names);
    EXPECT_EQ(2u, names.size());
    EXPECT_TRUE(ContainerUtils::Contains(names, "log"));
    EXPECT_TRUE(ContainerUtils::Contains(names, "server"));

    parser->GetSectionKeys("server", names);
    EXPECT_EQ(3u, names.size());
    EXPECT_TRUE(ContainerUtils::Contains(names, "type"));
    EXPECT_TRUE(ContainerUtils::Contains(names, "port"));
    EXPECT_TRUE(ContainerUtils::Contains(names, "host"));
}

TEST(IniParserTest, BaseTest) {
    const char* ini =
        "[log]\n"
        "type=async\n"
        "level=debug\n"
        "name=ut\n"
        "[server]\n"
        ";http or tcp\n"
        "type=http\n"
        "port=8080\n"
        "host=localhost\n";
    const char* filename = "test.ini";
    fstream fs(filename, ios_base::out);
    fs.write(ini, strlen(ini));
    fs.flush();

    IniParser parser(filename);
    parser.AddCallback(ParseCb);
    EXPECT_TRUE(parser.Parse());
}

TEST(IniParserTest, ThreadTest) {
    const char* ini =
            "[log]\n"
                    "type=async\n"
                    "level=debug\n"
                    "name=ut\n"
                    "[server]\n"
                    ";http or tcp\n"
                    "type=http\n"
                    "port=8080\n"
                    "host=localhost\n";
    const char* filename = "test.ini";
    fstream fs(filename, ios_base::out);
    fs.write(ini, strlen(ini));
    fs.flush();

    IniParser parser(filename, false, 1);
    EXPECT_TRUE(parser.Parse());
    EXPECT_STREQ("async", parser.Get<string>("log", "type", "tmp").c_str());
    EXPECT_STREQ("http", parser.Get<string>("server", "type", "def").c_str());
    EXPECT_STREQ("http", parser.Get<string>("server", "tYPe", "def").c_str());
    EXPECT_EQ(8080, parser.Get("server", "port", 111));
    EXPECT_EQ(111, parser.Get("server", "port1", 111));

    const char* ini2 =
            "[log]\n"
                    "type=sync\n"
                    "level=debug\n"
                    "name=ut\n"
                    "[server]\n"
                    ";http or tcp\n"
                    "type=http\n"
                    "port=8080\n"
                    "host=localhost\n";
    fstream fs2(filename, ios_base::out);
    fs2.write(ini2, strlen(ini2));
    fs2.flush();
    sleep(2);
    EXPECT_STREQ("sync", parser.Get<string>("log", "type", "tmp").c_str());
    parser.Stop();
}
