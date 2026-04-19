#ifndef HRPC_RPC_PROVIDER_H
#define HRPC_RPC_PROVIDER_H

#include <google/protobuf/service.h>
#include <google/protobuf/descriptor.h>

#include <string>
#include <memory>
#include <unordered_map>

#include "mprpc/export.h"

namespace hrpc
{
class ZkClient;

namespace net
{
class EventLoop;
class TcpServer;
class TcpConnection;
using TcpConnectionPtr = std::shared_ptr<TcpConnection>;
}

// 框架提供的专门发布rpc服务的网络对象类
class HRPC_API RpcProvider {
public:
    RpcProvider();
    ~RpcProvider();

    void NotifyService(google::protobuf::Service* service);

    // 启动rpc服务节点
    void Run();

private:
    void OnConnection(const hrpc::net::TcpConnectionPtr& conn);
    void OnMessage(const hrpc::net::TcpConnectionPtr&);

    // Closure的回调操作，用于序列化rpc的响应和网络发送
    void SendRpcResponse(const hrpc::net::TcpConnectionPtr&, google::protobuf::Message*);

private:
    std::unique_ptr<ZkClient> m_zkClientPtr;

    std::unique_ptr<hrpc::net::TcpServer> m_serverPtr;
    hrpc::net::EventLoop* m_eventLoop;

    // service服务类型
    struct ServiceInfo {
        google::protobuf::Service* m_service;       // 保存服务对象
        std::unordered_map<std::string, const google::protobuf::MethodDescriptor*> m_methodMap; // 保存服务方法
    };

    // 存储注册成功的服务对象和其服务方法的所有信息
    std::unordered_map<std::string, ServiceInfo> m_serviceMap;
}; 

} // namespace hrpc

#endif // HRPC_RPC_PROVIDER_H
