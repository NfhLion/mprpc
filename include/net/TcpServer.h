#ifndef HRPC_NET_TCPSERVER_H
#define HRPC_NET_TCPSERVER_H

#include "InetAddress.h"
#include "TcpConnection.h"
#include "noncopyable.h"
#include "Acceptor.h"

#include <unordered_map>
#include <string>
#include <memory>

namespace hrpc
{
namespace net
{

class EventLoop;

class TcpServer : public noncopyable {
public:
    TcpServer(EventLoop* loop,
              const InetAddress& listenAddr,
              const std::string& nameArg,
              bool reusePort = true);
    ~TcpServer();

    void start();

    EventLoop* getLoop() const { return loop_; }


    void setConnectionCallback(const ConnectionCallback& cb)
    { connectionCallback_ = cb; }

    /// Set message callback.
    /// Not thread safe.
    void setMessageCallback(const MessageCallback& cb)
    { messageCallback_ = cb; }

    /// Set write complete callback.
    /// Not thread safe.
    void setWriteCompleteCallback(const WriteCompleteCallback& cb)
    { writeCompleteCallback_ = cb; }

private:

    void newConnection(int sockfd, const InetAddress& peerAddr);
    void removeConnection(const TcpConnectionPtr& conn);

    typedef std::unordered_map<string, TcpConnectionPtr> ConnectionMap;

    EventLoop* loop_;
    const std::string name_;
    const std::string ipPort_;

    ConnectionCallback connectionCallback_;
    MessageCallback messageCallback_;
    WriteCompleteCallback writeCompleteCallback_;
    ConnectionMap connections_;

    std::unique_ptr<Acceptor> acceptor_;

    int nextConnId_;
};

} // namespace net
} // namespace hrpc

#endif // HRPC_NET_TCPSERVER_H