//
// Created by zlz on 2016/1/30.
//

#ifndef ZUTILS_TCP_CONNECTION_H
#define ZUTILS_TCP_CONNECTION_H

#include "common.h"
#include "inet_address.h"
#include "callbacks.h"

ZUTIL_NET_NAMESPACE_BEGIN

enum class ConnectionState {
    kInit,
    kConnecting,
    kConnected,
    kDisconnecting,
    kDisconnected
};


class EventLoop;

class Socket;

class Channel;

class TcpConnection {
public:
    TcpConnection(EventLoop* loop, const string& name, int32_t fd,
                  const INetAddress& localaddr,
                  const INetAddress& peeraddr, bool keepalive = true);

    ~TcpConnection();

    EventLoop* loop() const {
        return loop_;
    }

    string name() const {
        return name_;
    }

    const INetAddress& local_addr() const {
        return local_addr_;
    };

    const INetAddress& peer_addr() const {
        return peer_addr_;
    };

    ConnectionState state() const {
        return state_;
    }

    bool IsConnected() const;

    void Disconnect();

    void Connect();

    void set_connection_cb(const ConnectionCallback& cb) {
        connection_cb_ = cb;
    }

    void set_onmessage_cb(const OnMessageCallback& cb) {
        onmessage_cb_ = cb;
    }

    void set_writedone_cb(const WriteDoneCallback& cb) {
        writedone_cb_ = cb;
    }

    void set_close_cb(const CloseCallback& cb) {
        close_cb_ = cb;
    }

    void Send(const char* data, size_t size);

    string ToString() const {
        return name_;
    }

    static string ToString(ConnectionState state) {
        switch (state) {
            case ConnectionState::kInit:
                return "init";
            case ConnectionState::kConnecting:
                return "connecting";
            case ConnectionState::kConnected:
                return "connected";
            case ConnectionState::kDisconnecting:
                return "disconnecting";
            case ConnectionState::kDisconnected:
                return "disconnected";
        }
    }

    void OnConnectionCreated();

    void OnConnectionDestroyed();

private:

    void HandleRead();

    void HandleWrite();

    void HandleClose();

    void HandleError();

    void ChangeState(ConnectionState from, ConnectionState to);

    void DoDisconnect();

    void DoSend(const char* data, size_t size);

    EventLoop* loop_;
    string name_;
    int32_t fd_;
    INetAddress local_addr_;
    INetAddress peer_addr_;
    volatile ConnectionState state_;
    Channel* channel_;
    Socket* socket_;
    Buffer in_buf_;
    Buffer out_buf_;

    ConnectionCallback connection_cb_;
    OnMessageCallback onmessage_cb_;
    WriteDoneCallback writedone_cb_;
    CloseCallback close_cb_;

};

ZUTIL_NET_NAMESPACE_END

#endif //ZUTILS_TCP_CONNECTION_H
