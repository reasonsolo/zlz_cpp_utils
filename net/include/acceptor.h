//
// Created by zlz on 2016/1/30.
//

#ifndef ZUTILS_ACCEPTOR_H
#define ZUTILS_ACCEPTOR_H

#include "common.h"
#include "channel.h"
#include "zsocket.h"

ZUTIL_NET_NAMESPACE_BEGIN

class INetAddress;
class EventLoop;

class Acceptor {
public:
    typedef std::function<void(int32_t, const INetAddress&)> NewConnectionCallback;

    Acceptor(EventLoop* loop, const INetAddress& addr);

    ~Acceptor();

    void set_new_conn_cb(const NewConnectionCallback& cb) {
        new_conn_cb_ = cb;
    }

    bool is_listening() const {
        return is_listening_;
    }

    /*
     * call this in loop thread
     */
    void Listen();

private:

    void HandleRead();

    EventLoop* loop_;
    Channel channel_;
    Socket socket_;
    NewConnectionCallback new_conn_cb_;
    bool is_listening_;
};

ZUTIL_NET_NAMESPACE_END


#endif //ZUTILS_ACCEPTOR_H
