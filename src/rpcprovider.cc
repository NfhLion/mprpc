#include "rpcprovider.h"
#include "mprpcapplication.h"
#include "rpcheader.pb.h"

void RpcProvider::NotifyService(google::protobuf::Service* service) {

    ServiceInfo service_info;
    // 获取服务对象的描述信息
    const google::protobuf::ServiceDescriptor* pserviceDesc = service->GetDescriptor();
    // 获取服务名字
    std::string service_name = pserviceDesc->name();
    // 获取服务对象方法的数量
    int methodCot = pserviceDesc->method_count();

    for (int i = 0; i < methodCot; i++) {
        const google::protobuf::MethodDescriptor* pmethodDesc = pserviceDesc->method(i);
        std::string method_name = pmethodDesc->name();
        service_info.m_methodMap.insert({method_name, pmethodDesc});
    }
    service_info.m_service = service;
    m_serviceMap.insert({service_name, service_info});
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
    
    if (!conn->connected()) {
        conn->shutdown();
    }
}

// 读写事件回调
void RpcProvider::OnMessage(const muduo::net::TcpConnectionPtr& conn, 
                            muduo::net::Buffer* buffer,
                            muduo::Timestamp) {

    std::string recv_buf = buffer->retrieveAllAsString();

    // 读取4字节的头部大小
    uint32_t header_size = 0;
    recv_buf.copy((char*)&header_size, 4, 0);

    // 根据头部大小获取头部内容
    std::string rpc_header_str = recv_buf.substr(4, header_size);
    mprpc::RpcHeader rpcHeader;

    std::string service_name;
    std::string method_name;
    uint32_t args_size;
    if (rpcHeader.ParseFromString(rpc_header_str)) {
        // 反序列化成功
        service_name = rpcHeader.service_name();
        method_name = rpcHeader.method_name();
        args_size = rpcHeader.args_size();
    } else {
        std::cout << "rpc_header_str: " << rpc_header_str << ", parse error!" << std::endl;
        return;
    }

    std::string args_str = recv_buf.substr(4 + header_size, args_size);


    // 获取service对象和method对象
    auto it = m_serviceMap.find(service_name);
    if (it == m_serviceMap.end()) {
        std::cout << service_name << " is not exist! " << std::endl;
        return;
    }

    auto mit = it->second.m_methodMap.find(method_name);
    if (mit == it->second.m_methodMap.end()) {
        std::cout << service_name << ":" << method_name <<  " is not exist! " << std::endl;
        return;
    }
    
    google::protobuf::Service* service = it->second.m_service;
    const google::protobuf::MethodDescriptor* method = mit->second;

    // 生成rpc方法调用请求request和响应response参数
    google::protobuf::Message* request = service->GetRequestPrototype(method).New();
    if (!request->ParseFromString(args_str)) {
        std::cout << "request parse error, content: " << args_str << std::endl;
        return;
    }
    google::protobuf::Message* response = service->GetResponsePrototype(method).New();

    // 给下面的method方法的调用。绑定一个Closure的回调函数
    google::protobuf::Closure* done = google::protobuf::NewCallback<RpcProvider, 
                                                                    const muduo::net::TcpConnectionPtr&, 
                                                                    google::protobuf::Message*>
                                                                    (this, &RpcProvider::SendRpcResponse, conn, response);

    // 根据远端rpc请求，调用rpc节点发布服务的方法
    service->CallMethod(method, nullptr, request, response, done);
}

// Closure的回调操作，用于序列化rpc的响应和网络发送
void RpcProvider::SendRpcResponse(const muduo::net::TcpConnectionPtr& conn, google::protobuf::Message* response) {

    std::string response_str;
    if (response->SerializeToString(&response_str)) {
        conn->send(response_str);
        // 模拟http的短链接服务，由rpcprovider主动断开链接
        conn->shutdown();
    } else {
        std::cout << "serialize response_str error!" << std::endl;
    }

    conn->shutdown();
}