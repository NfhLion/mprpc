#pragma once

#include <memory>

#include <muduo/net/TcpServer.h>
#include <muduo/net/EventLoop.h>
#include <muduo/net/InetAddress.h>
#include <muduo/net/TcpConnection.h>
#include <google/protobuf/service.h>

// 框架提供的专门发布rpc服务的网络对象类
class RpcProvider {
public:
    void NotifyService(google::protobuf::Service* service);

    // 启动rpc服务节点
    void Run();

private:
    void OnConnection(const muduo::net::TcpConnectionPtr& conn);
    void OnMessage(const muduo::net::TcpConnectionPtr&, muduo::net::Buffer*, muduo::Timestamp);

private:
    muduo::net::EventLoop m_eventLoop;
}; 