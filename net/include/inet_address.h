//
// Created by zlz on 2016/1/17.
//

#ifndef ZUTILS_INETADDRESS_H
#define ZUTILS_INETADDRESS_H

#include "common.h"
#include <netinet/in.h>
#include "string_utils.h"

ZUTIL_NET_NAMESPACE_BEGIN

class INetAddress {
public:
    /*
     * format like 127.0.0.1:8080
     */
    explicit INetAddress(const string& addr, bool ipv6 = false);

    INetAddress(const string& ip = "", const int32_t port = 0, bool ipv6 = false);

    explicit INetAddress(const struct sockaddr_in& addr);

    explicit INetAddress(const struct sockaddr_in6& addr);

    const struct sockaddr* GetSockAddr() const;

    socklen_t GetSockAddrLen() const;

    int32_t port() const {
        return port_;
    }

    string ip() const {
        return ip_;
    }

    string ip_port() const {
        return StringUtils::ToString(ip_, ":", port_);
    }

    static bool ParseIPAndPort(const string& addr, string& ip, int32_t& port);

    static string Addr2IP(const struct sockaddr_in& addr);

    static string Addr62IP(const struct sockaddr_in6& addr);

    void set_addr6(const struct sockaddr_in6& addr6) {
        addr6_ = addr6;
    }

    void set_addr4(const struct sockaddr_in& addr) {
        addr_ = addr;
    }

private:
    bool SetAddrIpv4(const string& ip, const int32_t port, struct sockaddr_in& addr);

    bool SetAddrIpv6(const string& ip, const int32_t port, struct sockaddr_in6& addr6);

    string ip_;
    int32_t port_;

    union {
        struct sockaddr_in addr_;
        struct sockaddr_in6 addr6_;
    };

};

ZUTIL_NET_NAMESPACE_END

#endif //ZUTILS_INETADDRESS_H
