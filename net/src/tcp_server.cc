//
// Created by zlz on 2016/1/30.
//

#include "tcp_server.h"
#include "tcp_connection.h"
#include "string_utils.h"
#include "log.h"

ZUTIL_NET_NAMESPACE_BEGIN

TcpServer::TcpServer(const INetAddress& bind_addr) :
        base_loop_(EventLoop::GetThreadLoop()),
        acceptor_(base_loop_, bind_addr),
        thread_num_(1),
        is_keep_alive_(true) {

}

TcpServer::~TcpServer() {
    Stop();
}

void TcpServer::Start() {
    assert(!acceptor_.is_listening());
    // base_loop_->Run(std::bind(&Acceptor::Listen, &acceptor_));
    base_loop_->Run(std::bind(&Acceptor::Listen, &acceptor_));
    loop_pool_.Start();
    base_loop_->Start();
}

void TcpServer::Stop() {
    loop_pool_.Stop();
    base_loop_->Stop();
}

void TcpServer::CreateNewConnection(int32_t fd, const INetAddress& peer_addr) {
    EventLoop* next_loop = loop_pool_.GetNextLoop();
    string conn_name = StringUtils::ToString(name_, "-conn@", peer_addr.ip_port());
    sockaddr sockaddr;
    if (!INetAddress::FdToLocalAddr(fd, &sockaddr)) {
        ERROR_LOG(ToString() << " cannot get local addr of fd " << fd);
    }
    INetAddress local_addr(&sockaddr);
    TcpConnection* conn = new TcpConnection(next_loop, conn_name, fd, local_addr, peer_addr, is_keep_alive_);
    connection_map_[conn_name] = conn;
    conn->set_close_cb(close_cb_);
    conn->set_connection_cb(connection_cb_);
    conn->set_writedone_cb(writedone_cb_);
    conn->set_onmessage_cb(onmessage_cb_);
    next_loop->Run(std::bind(&TcpConnection::OnConnectionCreated, conn));
}

void TcpServer::RemoveConnection(TcpConnection* conn) {
    if (!conn->loop()->IsInLoop()) {
        connection_map_.erase(conn->name());
        conn->loop()->Run(std::bind(&TcpServer::RemoveConnection, this, conn));
    } else {
        conn->loop()->AssertInLoop();
        conn->OnConnectionDestroyed();
    }
}

ZUTIL_NET_NAMESPACE_END
