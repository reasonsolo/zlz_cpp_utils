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

    void set_thread_num(int32_t num) {
        thread_num_ = num;
    }

    void set_keep_alive(bool on) {
        is_keep_alive_ = on;
    }

    void Start();

    void Stop();

    string ToString() const {
        return name_;
    }

protected:

    void DefaultConnectionCallback(TcpConnection* conn);

    void DefaultOnMessageCallback(TcpConnection* conn, Buffer* buffer);

    void DefaultWriteDoneCallback(TcpConnection* conn);

    void DefaultCloseCallback(TcpConnection* conn);

    void CreateNewConnection(int32_t fd, const INetAddress& addr);

    void RemoveConnection(TcpConnection* conn);

    EventLoop* base_loop_;
    EventLoopThreadPool loop_pool_;
    Acceptor acceptor_;
    uint32_t thread_num_;
    bool is_keep_alive_;
    string name_;

    ConnectionCallback connection_cb_;
    OnMessageCallback onmessage_cb_;
    WriteDoneCallback writedone_cb_;
    CloseCallback close_cb_;

    ConnnectionMap connection_map_;

};

ZUTIL_NET_NAMESPACE_END

#endif //ZUTILS_TCP_SERVER_H
