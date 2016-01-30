//
// Created by zlz on 2016/1/30.
//

#include "tcp_connection.h"
#include "channel.h"
#include "log.h"

ZUTIL_NET_NAMESPACE_BEGIN

const size_t kMaxReadBufLen = 128 * 1024;

template<>
const char* EnumStrings<ConnectionState>::data = {"Init", "Connecting", "Connected", "Disconnected"};

TcpConnection::TcpConnection(EventLoop* loop, const string& name, int32_t fd,
                             const INetAddress& localaddr,
                             const INetAddress& peeraddr, bool keepalive) :
        loop_(loop),
        name_(name),
        fd_(fd),
        local_addr_(local_addr),
        peer_addr_(peer_addr),
        state_(ConnectionState::kInit),
        channel_(new Channel(loop_, fd_)),
        socket_(new Socket(fd_)) {
    channel_->set_read_cb(std::bind(&TcpConnection::HandleRead, this));
    channel_->set_write_cb(std::bind(&TcpConnection::HandleWrite, this));
    channel_->set_close_cb(std::bind(&TcpConnection::HandleClose, this));
    channel_->set_error_cb(std::bind(&TcpConnection::HandleError, this));
    socket_->SetKeepAlive(keepalive);
}

TcpConnection::~TcpConnection() {
    delete channel_;
    delete socket_;
}

bool TcpConnection::IsConnected() const {
    return state_ == ConnectionState::kConnected;
}

void TcpConnection::HandleRead() {
    loop_->AssertInLoop();
    Buffer read_buf;
    read_buf.reserve(kMaxReadBufLen);
    size_t size = read(channel_->fd(), &(read_buf[0]), kMaxReadBufLen);
    if (size > 0) {
        read_buf.resize(size);
        DEBUG_LOG(ToString() << " reads " << size << " from " << channel_->ToString());
        onmessage_cb_(this, read_buf);
    } else if (size == 0) {
        HandleClose();
    } else {
        handleError();
    }
}

void TcpConnection::HandleClose() {
    loop_->AssertInLoop();
    DEBUG_LOG(ToString() << " is closing");
    ChangeState(ConnectionState::kConnected, ConnectionState::kDisconnected);
    channel_->DisableAll();
    connection_cb_(this);
    close_cb_(this);
}

void TcpConnection::HandleWrite() {
    loop_->AssertInLoop();
    if (channel_->IsWritable()) {


    } else {
        WARN_LOG(ToString() << " cannot write to channel");
    }
}

void TcpConnection::ChangeState(ConnectionState from, ConnectionState to) {
    if (__sync_bool_compare_and_swap(&state_, from, to)) {
        DEBUG_LOG(ToString() << " change state from " << EnumToString(from) << " to "
                  << EnumToString(To));
    } else {
        FATAL_LOG(ToString() << " cannot change state from " << EnumToString(from) << " to "
                  << EnumToString(To) << ", current state is " << EnumToString(state_));
    }
}

void TcpConnection::OnConnectionCreated() {
    loop_->AssertInLoop();
    DEBUG_LOG(ToString() << " is connected");
    ChangeState(ConnectionState::kConnecting, ConnectionState::kConnected);
    channel_->EnableRead();
    connection_cb_(this);
}

void TcpConnection::OnConnectionDestroyed() {
    loop_->AssertInLoop();
    DEBUG_LOG(ToString() << " is destroyed");
    if (state_ == ConnectionState::kConnected) {
        channel_->DisableAll();
        connection_cb_(this);
    }
    ChangeState(state_, ConnectionState::kDisconnected);
    channel_->Remove();
}

ZUTIL_NET_NAMESPACE_END