//
// Created by zlz on 2016/1/30.
//

#include "tcp_connection.h"
#include "channel.h"
#include "log.h"
#include "zsocket.h"
#include "buffer.h"

ZUTIL_NET_NAMESPACE_BEGIN

TcpConnection::TcpConnection(EventLoop* loop, const string& name, int32_t fd,
                             const INetAddress& local_addr,
                             const INetAddress& peer_addr, bool keepalive) :
        loop_(loop),
        name_(name),
        fd_(fd),
        local_addr_(local_addr),
        peer_addr_(peer_addr),
        state_(ConnectionState::kInit),
        channel_(new Channel(loop_, fd_)),
        socket_(new Socket(fd_)) {
    assert(loop != nullptr);
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
    int32_t error = 0;
    int32_t size = in_buf_.ReadFromFd(channel_->fd(), error);
    if (size > 0) {
        DEBUG_LOG(ToString() << " reads " << size << " from " << channel_->ToString());
        onmessage_cb_(this, &in_buf_);
    } else if (size == 0) {
        HandleClose();
    } else {
        HandleError();
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
        ssize_t n = write(channel_->fd(), out_buf_.Readable(), out_buf_.ReadableSize());
        if (n >= 0) {
            out_buf_.Take(n);
            if (out_buf_.ReadableSize() == 0 || n == 0) {
                channel_->DisableWrite();
                writedone_cb_(this);
                if (state_ == ConnectionState::kDisconnecting) {
                    DoDisconnect();
                }
            }
        } else {
            ERROR_LOG(ToString() << " cannot write");
        }
    } else {
        WARN_LOG(ToString() << " cannot write to channel");
    }
}

void TcpConnection::HandleError() {
    int32_t error = socket_->GetError();
    ERROR_LOG(ToString() << " get error code " << error);
}

void TcpConnection::ChangeState(ConnectionState from, ConnectionState to) {
    if (__sync_bool_compare_and_swap(&state_, from, to)) {
        DEBUG_LOG(ToString() << " change state from " << ToString(from) << " to "
                  << ToString(to));
    } else {
        FATAL_LOG(ToString() << " cannot change state from " << ToString(from) << " to "
                  << ToString(to) << ", current state is " << ToString(state_));
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

void TcpConnection::Disconnect() {
    if (state_ == ConnectionState::kConnected) {
        ChangeState(ConnectionState::kConnected, ConnectionState::kDisconnecting);
        loop_->Run(std::bind(&TcpConnection::DoDisconnect, this));
    }
}

void TcpConnection::DoDisconnect() {
    loop_->AssertInLoop();
    if (!channel_->IsWritable()) {
        socket_->ShutDownWrite();
    }
}

void TcpConnection::Send(const char* data, size_t size) {
    if (!loop_->IsInLoop()) {
        loop_->Run(std::bind(&TcpConnection::DoSend, this, data, size));
    }
}

void TcpConnection::DoSend(const char* data, size_t size) {
    // fixme: what will happen if data is an array on stack?
    loop_->AssertInLoop();
    if (state_ == ConnectionState::kDisconnected) {
        ERROR_LOG(ToString() << " is disconnected, abort sending");
        return;
    }
    ssize_t written = 0;
    size_t remaining = size;
    if (out_buf_.ReadableSize() == 0 && !channel_->IsWritable()) {
        // send directly if there is not any other writing operation and send buf is empty
        written = write(channel_->fd(), data, size);
        if (written >= 0) {
            if (static_cast<size_t>(written) == size) {
                if (writedone_cb_) {
                    writedone_cb_(this);
                }
            } else {
                remaining = size - written;
            }
        } else {
            ERROR_LOG(ToString() << " cannot write, get error " << errno);
        }
    }
    if (remaining > 0) {
        out_buf_.Append(data + written, remaining);
        if (channel_->IsWritable()) {
            channel_->EnableWrite();
        }
    }

}

ZUTIL_NET_NAMESPACE_END