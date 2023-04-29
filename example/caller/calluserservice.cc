#include <iostream>

#include "mprpcapplication.h"
#include "mprpcchannel.h"
#include "user.pb.h"

using namespace hrpc;

int main(int argc, char* argv[]) {

    // 初始化框架
    MprpcApplication::Init(argc, argv);

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
    stub.Register(nullptr, &req, &rsp, nullptr); 

    // 一次rpc调用完成，读调用的结果
    if (0 == rsp.result().errcode())
    {
        std::cout << "rpc register response success:" << rsp.success() << std::endl;
    }
    else
    {
        std::cout << "rpc register response error : " << rsp.result().errmsg() << std::endl;
    }

    return 0;
}