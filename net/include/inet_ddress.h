//
// Created by zlz on 2016/1/17.
//

#ifndef ZUTILS_INETADDRESS_H
#define ZUTILS_INETADDRESS_H

#include "common.h"
#include <netinet/in.h>

ZUTIL_NET_NAMESPACE_BEGIN

class INetAddress {
public:
    explicit INetAddress(const string& addr);

    explicit INetAddress(const struct sockaddr_in& addr);

    // TODO: IPV6 support

    INetAddress(const string& ip, const int32_t port);

    int32_t port() const;

    string ip() const;

    string ip_port() const;

    sa_family_t family() const;

private:


};

ZUTIL_NET_NAMESPACE_END

#endif //ZUTILS_INETADDRESS_H
