//
// Created by zlz on 2016/1/30.
//

#include "tcp_server.h"
#include "tcp_connection.h"
#include "string_utils.h"
#include "log.h"

ZUTIL_NET_NAMESPACE_BEGIN

using std::placeholders::_1;
using std::placeholders::_2;

TcpServer::TcpServer(const INetAddress& bind_addr) :
        base_loop_(EventLoop::GetThreadLoop()),
        acceptor_(base_loop_, bind_addr),
        thread_num_(1),
        is_keep_alive_(true),
        name_(string("TcpServer@").append(bind_addr.ip_port())),
        connection_cb_(std::bind(&TcpServer::DefaultConnectionCallback, this, _1)),
        onmessage_cb_(std::bind(&TcpServer::DefaultOnMessageCallback, this, _1, _2)),
        writedone_cb_(std::bind(&TcpServer::DefaultWriteDoneCallback, this, _1)),
        close_cb_(std::bind(&TcpServer::DefaultCloseCallback, this, _1)) {
    loop_pool_.set_thread_size(thread_num_);
    acceptor_.set_new_conn_cb(std::bind(&TcpServer::CreateNewConnection, this, _1, _2));
}

TcpServer::~TcpServer() {
    Stop();
    delete base_loop_;
}

void TcpServer::Start() {
    DEBUG_LOG(ToString() << " is starting");
    assert(!acceptor_.is_listening());
    // base_loop_->Run(std::bind(&Acceptor::Listen, &acceptor_));
    acceptor_.Listen();
    loop_pool_.Start();
    base_loop_->Start();
}

void TcpServer::Stop() {
    loop_pool_.Stop();
    base_loop_->Stop();
}

void TcpServer::CreateNewConnection(int32_t fd, const INetAddress& peer_addr) {
    DEBUG_LOG(ToString() << " get new connection from fd " << fd << " " << peer_addr.ip_port());
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
        delete conn;
    }
}

void TcpServer::DefaultConnectionCallback(TcpConnection* conn) {
    DEBUG_LOG("TcpServer connection " << conn->ToString() << " connection callback");
}

void TcpServer::DefaultOnMessageCallback(TcpConnection* conn, Buffer* buffer) {
    DEBUG_LOG("TcpServer connection " << conn->ToString() << " get message: " << buffer->TakeAllString());
}

void TcpServer::DefaultWriteDoneCallback(TcpConnection* conn) {
    DEBUG_LOG("TcpServer connection " << conn->ToString() << " write done callback");
}

void TcpServer::DefaultCloseCallback(TcpConnection* conn) {
    DEBUG_LOG("TcpServer connection " << conn->ToString() << " closed");
}

ZUTIL_NET_NAMESPACE_END
