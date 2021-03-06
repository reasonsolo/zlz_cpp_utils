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
    explicit INetAddress(const string& addr = "", bool ipv6 = false);

    explicit INetAddress(const string& ip, const int32_t port, bool ipv6 = false);

    explicit INetAddress(const struct sockaddr_in& addr);

    explicit INetAddress(const struct sockaddr_in6& addr);

    explicit INetAddress(struct sockaddr* addr);

    INetAddress(const INetAddress& inaddr);

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

    static string AddrToIP(const struct sockaddr_in& addr);

    static string Addr6ToIP(const struct sockaddr_in6& addr);

    void set_addr6(const struct sockaddr_in6& addr6) {
        ipv6_ = true;
        addr6_ = addr6;
    }

    void set_addr4(const struct sockaddr_in& addr) {
        ipv6_ = false;
        addr_ = addr;
    }

    static bool FdToLocalAddr(int32_t fd, struct sockaddr* sockaddr);

private:
    bool SetAddrIpv4(const string& ip, const int32_t port, struct sockaddr_in& addr);

    bool SetAddrIpv6(const string& ip, const int32_t port, struct sockaddr_in6& addr6);

    string ip_;
    int32_t port_;
    struct sockaddr_in addr_;
    struct sockaddr_in6 addr6_;
    bool ipv6_;

};

ZUTIL_NET_NAMESPACE_END

#endif //ZUTILS_INETADDRESS_H
