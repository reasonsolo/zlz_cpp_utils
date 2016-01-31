//
// Created by zlz on 2016/1/30.
//

#ifndef ZUTILS_TCP_SERVER_H
#define ZUTILS_TCP_SERVER_H

#include "common.h"
#include "inet_address.h"
#include "acceptor.h"
#include "event_loop.h"
#include "event_loop_thread_pool.h"
#include "tcp_connection.h"

ZUTIL_NET_NAMESPACE_BEGIN

class TcpConnection;

class TcpServer {
public:
    typedef map<string, TcpConnection*> ConnnectionMap;
    TcpServer(const INetAddress& bind_addr);

    ~TcpServer();

    void set_thread_num(int32_t num);

    void Start();

    void Stop();
private:

    void CreateNewConnection(int32_t fd, const INetAddress& addr);

    EventLoop* base_loop_;
    EventLoopThreadPool loop_pool_;
    Acceptor acceptor_;
    int32_t thread_num_;

    ConnectionCallback connection_cb_;
    OnMessageCallback onmessage_cb_;
    WriteDoneCallback writedone_cb_;

    ConnnectionMap connection_map_;

};

ZUTIL_NET_NAMESPACE_END

#endif //ZUTILS_TCP_SERVER_H
