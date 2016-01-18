//
// Created by zlz on 2016/1/18.
//

#include <gtest/gtest.h>
#include "zfile.h"
#include "file_utils.h"
#include "time_utils.h"
#include "common.h"

ZUTIL_NAMESPACE_USE;

TEST(FileTest, ReadWriteTest) {
    // TODO: fix this ut
    string content = "hello world!";
    string filename = "/cygdrive/c/Users/zlz/ClionProjects/zutils/utils/test/tst.tmp";
    File wr_file(filename);
    EXPECT_TRUE(wr_file.SetReadWrite().SetCreate().Open());
    EXPECT_TRUE(wr_file.Write(content.c_str(), content.size()) > 0);
    wr_file.Flush();
    string readed;
    EXPECT_EQ(content.size(), wr_file.ReadAll(readed));
    // EXPECT_EQ(TimeUtils::GetTimestampInMS() / 1000, wr_file.GetModificationTime() / 1000);
    EXPECT_STREQ(content.c_str(), readed.c_str());
}


TEST(FileUtilsTest, DirnameTest) {
    string dir, file;
    FileUtils::GetDirectoryAndFile("/etc/hosts", dir, file);
    EXPECT_STREQ("/etc", dir.c_str());
    EXPECT_STREQ("hosts", file.c_str());

    FileUtils::GetDirectoryAndFile("/usr/bin/", dir, file);
    EXPECT_STREQ("/usr/bin", dir.c_str());
    EXPECT_TRUE(file.empty());

    FileUtils::GetDirectoryAndFile("usr.txt", dir, file);
    EXPECT_STREQ("usr.txt", file.c_str());
    EXPECT_TRUE(dir.empty());
}