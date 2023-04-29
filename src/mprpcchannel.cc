#include "mprpcchannel.h"
#include "rpcheader.pb.h"
#include "mprpcapplication.h"

#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <errno.h>

using namespace hrpc;

 void MprpcChannel::CallMethod(const google::protobuf::MethodDescriptor* method,
                               google::protobuf::RpcController* controller,
                               const google::protobuf::Message* request,
                               google::protobuf::Message* response, 
                               google::protobuf::Closure* done)
{
    std::string service_name = method->service()->name();
    std::string method_name = method->name();

    uint32_t args_size = 0;
    std::string args_str;
    if (request->SerializeToString(&args_str)) {
        args_size = args_str.size();
    } else {
        std::cout << "serialize request error" << std::endl;
        return;
    }

    mprpc::RpcHeader rpcHeader;
    rpcHeader.set_service_name(service_name);
    rpcHeader.set_method_name(method_name);
    rpcHeader.set_args_size(args_size);

    uint32_t header_size = 0;
    std::string rpc_header_str;
    if (rpcHeader.SerializeToString(&rpc_header_str)) {
        header_size = rpc_header_str.size();
    } else {
        controller->SetFailed("serialize rpc header error");
        return;
    }

    // 组织带发送的rpc请求字符串
    std::string send_rpc_str;
    send_rpc_str.insert(0, std::string((char*)&header_size, 4)); // header_size
    send_rpc_str += rpc_header_str; // rpcheader
    send_rpc_str += args_str; // args                                // args_str
    
    // tcp编程, 创建socket

    int clientfd = socket(AF_INET, SOCK_STREAM, 0);
    if (-1 == clientfd) {
        controller->SetFailed("create socket error");
        return;
    }

    // std::string ip = MprpcApplication::GetInstance().GetConfig().Load("rpcserverip");
    // uint16_t port = atoi(MprpcApplication::GetInstance().GetConfig().Load("rpcserverport").c_str());
    std::string zk_host = MprpcApplication::GetInstance().GetConfig().Load("zookeeperip");
    std::string zk_port = MprpcApplication::GetInstance().GetConfig().Load("zookeeperport");
    m_zkClientPtr = std::make_unique<ZkClient>();
    m_zkClientPtr->Start(zk_host, zk_port);
    //  /UserServiceRpc/Login
    std::string method_path = "/" + service_name + "/" + method_name;
    // 127.0.0.1:8000
    std::string host_data = m_zkClientPtr->GetData(method_path.c_str());
    if (host_data == "")
    {
        controller->SetFailed(method_path + " is not exist!");
        return;
    }
    int idx = host_data.find(":");
    if (idx == -1)
    {
        controller->SetFailed(method_path + " address is invalid!");
        return;
    }
    std::string ip = host_data.substr(0, idx);
    uint16_t port = atoi(host_data.substr(idx+1, host_data.size()-idx).c_str()); 

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = inet_addr(ip.c_str());

    if (-1 == connect(clientfd, (struct sockaddr*)&server_addr, sizeof(server_addr))) {
        char errtxt[512] = "\0";
        sprintf(errtxt, "connet error! error: %d", errno);
        controller->SetFailed(errtxt);
        close(clientfd);
        return;
    }

    // 发送rpc请求
    if (-1 == send(clientfd, send_rpc_str.c_str(), send_rpc_str.size(), 0)) {
        char errtxt[512] = "\0";
        sprintf(errtxt, "send error! error: %d", errno);
        controller->SetFailed(errtxt);
        close(clientfd);
        return;
    }

    // 接收rpc请求的响应值
    char recv_buf[1024] = "\0";
    int recv_size = 0;
    if (-1 == (recv_size = recv(clientfd, recv_buf, 1024, 0))) {
        char errtxt[512] = "\0";
        sprintf(errtxt, "recv error! error: %d", errno);
        controller->SetFailed(errtxt);
        close(clientfd);
        return;
    }

    std::string response_str(recv_buf, 0, recv_size);
    if (!response->ParseFromArray(recv_buf, recv_size)) {
        char errtxt[512] = "\0";
        sprintf(errtxt, "parse error! recv_buf: %s", recv_buf);
        controller->SetFailed(errtxt);
    }
    close(clientfd);
    return;
}