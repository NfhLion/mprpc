#pragma once

#include <muduo/net/TcpServer.h>
#include <muduo/net/EventLoop.h>
#include <muduo/net/InetAddress.h>
#include <muduo/net/TcpConnection.h>
#include <google/protobuf/service.h>
#include <google/protobuf/descriptor.h>

#include <string>
#include <functional>
#include <unordered_map>

// 框架提供的专门发布rpc服务的网络对象类
class RpcProvider {
public:
    void NotifyService(google::protobuf::Service* service);

    // 启动rpc服务节点
    void Run();

private:
    void OnConnection(const muduo::net::TcpConnectionPtr& conn);
    void OnMessage(const muduo::net::TcpConnectionPtr&, muduo::net::Buffer*, muduo::Timestamp);

    // Closure的回调操作，用于序列化rpc的响应和网络发送
    void SendRpcResponse(const muduo::net::TcpConnectionPtr&, google::protobuf::Message*);

private:
    muduo::net::EventLoop m_eventLoop;

    // service服务类型
    struct ServiceInfo {
        google::protobuf::Service* m_service;       // 保存服务对象
        std::unordered_map<std::string, const google::protobuf::MethodDescriptor*> m_methodMap; // 保存服务方法
    };

    // 存储注册成功的服务对象和其服务方法的所有信息
    std::unordered_map<std::string, ServiceInfo> m_serviceMap;
}; 