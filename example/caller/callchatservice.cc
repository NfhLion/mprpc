#include <iostream>

#include "mprpcapplication.h"
#include "mprpcchannel.h"
#include "mprpccontroller.h"
#include "user.pb.h"
#include "friend.pb.h"

using namespace hrpc;

void UseUserServicePrcTest() {
    // ------------ 远程调用Login
    fixbug::UserServiceRpc_Stub stub(new MprpcChannel());
    fixbug::LoginRequest request;
    request.set_name("zhang san");
    request.set_pwd("123456");

    fixbug::LoginResponse response;

    // 执行rpc远程调用
    stub.Login(nullptr, &request, &response, nullptr);

    // 一次rpc远程调用结束
    if (response.result().errcode() == 0) {
        std::cout << "rpc login response success: " << response.success() << std::endl;
    } else {
        std::cout << "rpc login response error: " << response.result().errmsg() << std::endl;
    }

    // 演示调用远程发布的rpc方法Register
    fixbug::RegisterRequest req;
    req.set_id(2000);
    req.set_name("mprpc");
    req.set_pwd("666666");
    fixbug::RegisterResponse rsp;

    // 以同步的方式发起rpc调用请求，等待返回结果
    MprpcController controller;
    stub.Register(&controller, &req, &rsp, nullptr); 

    // 一次rpc调用完成，读调用的结果
    if (controller.Failed())
    {
        std::cout << controller.ErrorText() << std::endl;
    } else {
        if (0 == rsp.result().errcode())
        {
            std::cout << "rpc register response success:" << rsp.success() << std::endl;
        }
        else
        {
            std::cout << "rpc register response error : " << rsp.result().errmsg() << std::endl;
        }
    }
}

void UseFriendServiceRpcTest() {
    // 演示调用远程发布的rpc方法Login
    fixbug::FiendServiceRpc_Stub stub(new MprpcChannel());
    // rpc方法的请求参数
    fixbug::GetFriendsListRequest request;
    request.set_userid(1000);
    // rpc方法的响应
    fixbug::GetFriendsListResponse response;
    // 发起rpc方法的调用  同步的rpc调用过程  MprpcChannel::callmethod
    MprpcController controller;
    stub.GetFriendsList(&controller, &request, &response, nullptr); // RpcChannel->RpcChannel::callMethod 集中来做所有rpc方法调用的参数序列化和网络发送

    // 一次rpc调用完成，读调用的结果
    if (controller.Failed())
    {
        std::cout << controller.ErrorText() << std::endl;
    }
    else
    {
        if (0 == response.result().errcode())
        {
            std::cout << "rpc GetFriendsList response success!" << std::endl;
            int size = response.friends_size();
            for (int i=0; i < size; ++i)
            {
                std::cout << "index:" << (i+1) << " name:" << response.friends(i) << std::endl;
            }
        }
        else
        {
            std::cout << "rpc GetFriendsList response error : " << response.result().errmsg() << std::endl;
        }
    }
}

int main(int argc, char* argv[]) {

    // 初始化框架
    MprpcApplication::Init(argc, argv);

    UseUserServicePrcTest();
    UseFriendServiceRpcTest();

    return 0;
}