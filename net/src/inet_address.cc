//
// Created by zlz on 2016/1/28.
//

#include "inet_address.h"
#include "string_utils.h"
#include "numeric_utils.h"
#include <sys/socket.h>
#include <arpa/inet.h>

ZUTIL_NET_NAMESPACE_BEGIN

INetAddress::INetAddress(const string& addr, bool ipv6) {
    ParseIPAndPort(addr, ip_, port_);
    if (ipv6) {
        SetAddrIpv6(ip_, port_, addr6_);
    } else {
        SetAddrIpv4(ip_, port_, addr_);
    }
}

INetAddress::INetAddress(const struct sockaddr_in& addr) : addr_(addr) {
    ip_ = Addr2IP(addr_);
    port_ = ntohs(addr_.sin_port);
}

INetAddress::INetAddress(const struct sockaddr_in6& addr) : addr6_(addr) {
    ip_ = Addr62IP(addr6_);
    port_ = ntohs(addr6_.sin6_port);
}

INetAddress::INetAddress(const string& ip, const int32_t port, bool ipv6) : ip_(ip), port_(port) {
    if (ipv6) {
        SetAddrIpv6(ip_, port_, addr6_);
    } else {
        SetAddrIpv4(ip_, port_, addr_);
    }
}

const struct sockaddr* INetAddress::GetSockAddr() const {
    return static_cast<const struct sockaddr*>(implict_cast<const void*>(&addr6_));
}

socklen_t INetAddress::GetSockAddrLen() const {
    return static_cast<socklen_t>(sizeof(addr6_));
}

bool INetAddress::ParseIPAndPort(const string& addr, string& ip, int32_t& port) {
    string port_str;
    return StringUtils::Split(addr, ":", ip, port_str) && NumericUtils::String2Int32(port_str, port);
}

bool INetAddress::SetAddrIpv4(const string& ip, const int32_t port, struct sockaddr_in& addr) {
    bzero(&addr_, sizeof(addr));
    addr_.sin_family = AF_INET;
    addr_.sin_port = htons(static_cast<uint16_t>(port));
    return inet_pton(AF_INET, ip.c_str(), &addr_.sin_addr) == 1;
}


bool INetAddress::SetAddrIpv6(const string& ip, const int32_t port, struct sockaddr_in6& addr) {
    bzero(&addr_, sizeof(addr));
    addr6_.sin6_family = AF_INET6;
    addr6_.sin6_port = htons(static_cast<uint16_t>(port));
    return inet_pton(AF_INET6, ip.c_str(), &addr_.sin_addr) == 1;
}

string INetAddress::Addr2IP(const struct sockaddr_in& addr) {
    assert(addr.sin_family == AF_INET);
    char buf[128] = {0};
    inet_ntop(addr.sin_family, &addr.sin_addr, buf, sizeof(buf));
    return string(buf);
}

string INetAddress::Addr62IP(const struct sockaddr_in6& addr) {
    assert(addr.sin6_family == AF_INET6);
    char buf[128] = {0};
    inet_ntop(addr.sin6_family, &addr.sin6_addr, buf, sizeof(buf));
    return string(buf);
}

ZUTIL_NET_NAMESPACE_END