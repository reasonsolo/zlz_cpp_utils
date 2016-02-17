//
// Created by zlz on 2016/1/13.
//

#include "string_utils.h"
#include <gtest/gtest.h>
#include "container_utils.h"

ZUTIL_NAMESPACE_USE;

TEST(StringUtilsTest, SplitTest) {
    string a0 = "1,2,3,4,5";
    string a1 = "1,,2,3,4,5";
    vector<string> tokens_a;
    vector<string> tokens_ae {"1", "2", "3", "4", "5"};
    EXPECT_EQ(5u, StringUtils::Split(a0, ",", tokens_a));
    EXPECT_TRUE(ContainerUtils::CompareContainer(tokens_a, tokens_ae));
    EXPECT_EQ(5u, StringUtils::Split(a1, ",", tokens_a));
    EXPECT_TRUE(ContainerUtils::CompareContainer(tokens_a, tokens_ae));
}

TEST(StringUtilsTest, MapSplitTest) {
    string a0 = "a=1,b=2,c=3,d=4,e=5";
    string a1 = "a=1,b=2,c=3,d=4,e=5,f";
    map<string, string> tokens_a;
    EXPECT_EQ(5u, StringUtils::Split(a0, ",", "=", tokens_a));
    char k, v;
    for (k = 'a', v= '1'; k <= 'e'; k++, v++) {
        EXPECT_STREQ(string(1, v).c_str(), tokens_a[string(1, k)].c_str());
    }
    tokens_a.clear();
    EXPECT_EQ(5u, StringUtils::Split(a1, ",", "=", tokens_a));
    for (k = 'a', v= '1'; k <= 'e'; k++, v++) {
        EXPECT_STREQ(string(1, v).c_str(), tokens_a[string(1, k)].c_str());
    }
}

