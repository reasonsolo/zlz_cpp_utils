//
// Created by zlz on 2016/1/28.
//

#include "inet_address.h"
#include "string_utils.h"
#include "numeric_utils.h"
#include <sys/socket.h>
#include <arpa/inet.h>

ZUTIL_NET_NAMESPACE_BEGIN

INetAddress::INetAddress(const string& addr, bool ipv6): ipv6_(ipv6) {
    ParseIPAndPort(addr, ip_, port_);
    if (ipv6_) {
        SetAddrIpv6(ip_, port_, addr6_);
    } else {
        SetAddrIpv4(ip_, port_, addr_);
    }
}

INetAddress::INetAddress(const struct sockaddr_in& addr) : addr_(addr), ipv6_(false) {
    ip_ = AddrToIP(addr_);
    port_ = ntohs(addr_.sin_port);
}

INetAddress::INetAddress(const struct sockaddr_in6& addr) : addr6_(addr), ipv6_(true) {
    ip_ = Addr6ToIP(addr6_);
    port_ = ntohs(addr6_.sin6_port);
}

INetAddress::INetAddress(const string& ip, const int32_t port, bool ipv6) : ip_(ip), port_(port), ipv6_(ipv6) {
    if (ipv6_) {
        SetAddrIpv6(ip_, port_, addr6_);
    } else {
        SetAddrIpv4(ip_, port_, addr_);
    }
}

INetAddress::INetAddress(const INetAddress& inaddr) {
    ipv6_ = inaddr.ipv6_;
    ip_ = inaddr.ip_;
    port_ = inaddr.port_;
    addr_ = inaddr.addr_;
    addr6_ = inaddr.addr6_;
}

INetAddress::INetAddress(struct sockaddr* addr) {
    if (addr->sa_family == AF_INET) {
        memcpy(&addr_, addr, sizeof(addr_));
        ip_ = AddrToIP(addr_);
        port_ = ntohs(addr_.sin_port);
    } else if (addr->sa_family == AF_INET6){
        memcpy(&addr6_, addr, sizeof(addr6_));
        ip_ = Addr6ToIP(addr6_);
        port_ = ntohs(addr6_.sin6_port);
    }
}

const struct sockaddr* INetAddress::GetSockAddr() const {
    if (ipv6_) {
        return static_cast<const struct sockaddr*>(implict_cast<const void*>(&addr6_));
    } else {
        return static_cast<const struct sockaddr*>(implict_cast<const void*>(&addr_));
    }
}

socklen_t INetAddress::GetSockAddrLen() const {
    return static_cast<socklen_t>(ipv6_ ? sizeof(addr6_) : sizeof(addr_));
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

string INetAddress::AddrToIP(const struct sockaddr_in& addr) {
    assert(addr.sin_family == AF_INET);
    char buf[128] = {0};
    inet_ntop(addr.sin_family, &addr.sin_addr, buf, sizeof(buf));
    return string(buf);
}

string INetAddress::Addr6ToIP(const struct sockaddr_in6& addr) {
    assert(addr.sin6_family == AF_INET6);
    char buf[128] = {0};
    inet_ntop(addr.sin6_family, &addr.sin6_addr, buf, sizeof(buf));
    return string(buf);
}

bool INetAddress::FdToLocalAddr(int32_t fd, struct sockaddr* sockaddr) {
    socklen_t addr_len = sizeof(struct sockaddr);
    if (getsockname(fd, sockaddr, &addr_len) != 0) {
        return false;
    }
    return true;
}

ZUTIL_NET_NAMESPACE_END