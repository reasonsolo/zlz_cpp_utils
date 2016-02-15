//
// Created by zlz on 2016/2/15.
//

#include <gtest/gtest.h>
#include "tcp_server.h"
#include "inet_address.h"

ZUTIL_NET_NAMESPACE_USE;
ZUTIL_NAMESPACE_USE;

TEST(TcpServerTest, BasicTest) {
    INetAddress addr("0.0.0.0", 8089);
    TcpServer server(addr);
    server.Start();
}
