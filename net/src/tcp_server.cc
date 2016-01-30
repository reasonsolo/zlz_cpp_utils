//
// Created by zlz on 2016/1/30.
//

#include "tcp_server.h"


ZUTIL_NET_NAMESPACE_BEGIN

TcpServer::TcpServer(const INetAddress& bind_addr) :
        base_loop_(EventLoop::GetThreadLoop()),
        acceptor_(base_loop_, bind_addr),
        thread_num_(1) {

}

TcpServer::~TcpServer() {
    Stop();
}

void TcpServer::Start() {
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

void TcpServer::CreateNewConnection(int32_t fd, const INetAddress& addr) {

}

ZUTIL_NET_NAMESPACE_END
