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
    string content = "hello world!";
    string filename = "ut.tmp";
    File wr_file(filename);
    EXPECT_TRUE(wr_file.SetReadWrite().SetCreate().Open());
    string readed;
    EXPECT_TRUE(content.size(), wr_file.ReadAll(readed));
    EXPECT_TRUE(TimeUtils::GetTimeStamp(), wr_file.GetModificationTime() / 1000);
    EXPECT_STREQ(content.c_str(), readed.c_str());
}
