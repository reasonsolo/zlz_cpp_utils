//
// Created by zlz on 2016/1/11.
//

#include "common.h"
#include "container_utils.h"
#include <gtest/gtest.h>

ZUTIL_NAMESPACE_USE;


TEST(ContainerTest, CompareTest) {
    vector<int32_t> a {1, 2, 3, 4, 5};
    vector<int32_t> b {1, 2, 3, 4, 5};
    vector<int32_t> c {1, 2, 3, 4, 5, 6};

    EXPECT_TRUE(ContainerUtils::CompareContainer(a, b));
    EXPECT_FALSE(ContainerUtils::CompareContainer(a, c));
    EXPECT_FALSE(ContainerUtils::CompareContainer(c, a));
}

TEST(ContainerTest, CopyTest) {

    vector<int32_t> a {1, 2, 3, 4, 5};
    vector<int32_t> b;
    ContainerUtils::CopyContainer(a, b);

    EXPECT_TRUE(ContainerUtils::CompareContainer(a, b));
}

