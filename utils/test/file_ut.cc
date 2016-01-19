//
// Created by zlz on 2016/1/18.
//

#include <gtest/gtest.h>
#include "file_utils.h"
#include "time_utils.h"
#include "common.h"

ZUTIL_NAMESPACE_USE;


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