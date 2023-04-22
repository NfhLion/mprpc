#include "rpcprovider.h"
#include "mprpcapplication.h"

#include <string>
#include <functional>

void RpcProvider::NotifyService(google::protobuf::Service* service) {

}

// 启动rpc服务节点
void RpcProvider::Run() {

    std::string ip = MprpcApplication::GetInstance().GetConfig().Load("rpcserverip");
    uint16_t port = atoi(MprpcApplication::GetInstance().GetConfig().Load("rpcserverport").c_str());
    muduo::net::InetAddress address(ip, port);

    muduo::net::TcpServer server(&m_eventLoop, address, "RpcProvider");

    //绑定连接回调和消息读写回调
    server.setConnectionCallback(std::bind(&RpcProvider::OnConnection, this, std::placeholders::_1));
    server.setMessageCallback(std::bind(&RpcProvider::OnMessage, this, 
                              std::placeholders::_1,
                              std::placeholders::_2,
                              std::placeholders::_3));

    // 设置线程数量
    server.setThreadNum(4);

    std::cout << "RpcProvider start service at ip: " << ip << ", port: " << port << std::endl;

    // 启动网络服务
    server.start();
    m_eventLoop.loop();
}

// new socket连接回调
void RpcProvider::OnConnection(const muduo::net::TcpConnectionPtr& conn) {

}

// 读写事件回调
void RpcProvider::OnMessage(const muduo::net::TcpConnectionPtr& conn, muduo::net::Buffer*, muduo::Timestamp) {

}