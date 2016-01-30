//
// Created by zlz on 2016/1/30.
//

#include "acceptor.h"
#include "event_loop.h"
#include "log.h"

ZUTIL_NET_NAMESPACE_BEGIN

Acceptor::Acceptor(EventLoop* loop, const INetAddress& addr) :
        loop_(loop),
        channel_(loop, socket_.fd()),
        socket_(addr, true),
        is_listening_(false) {
    channel_.set_read_cb(std::bind(&Acceptor::HandleRead, this));
}

Acceptor::~Acceptor() {
    channel_.DisableAll();
    channel_.Remove();
}

void Acceptor::Listen() {
    loop_->AssertInLoop();
    DEBUG_LOG("listening on " << socket_.ToString());
    socket_.Listen();
    channel_.EnableRead();
    is_listening_ = true;

}

void Acceptor::HandleRead() {
    loop_->AssertInLoop();
    INetAddress peer_addr;
    int32_t conn_fd = socket_.Accept(peer_addr);
    if (conn_fd >= 0) {
        WARN_LOG("new connection fd " << conn_fd << " is available");
        if (new_conn_cb_) {
            new_conn_cb_(conn_fd, peer_addr);
        } else {
            WARN_LOG("no new connection callback is set");
        }
    } else {
        ERROR_LOG("cannot accept from " << socket_.ToString());
    }
}

ZUTIL_NET_NAMESPACE_END