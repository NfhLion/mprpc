#include <iostream>
#include <string>

#include "user.pb.h"
#include "mprpcapplication.h"
#include "rpcprovider.h"

/*
UserService目前是一个本地服务
*/
class UserService : public fixbug::UserServiceRpc {
public:
    bool Login(std::string name, std::string pwd) {
        std::cout << "doing local service: Login" << std::endl;
        std::cout << "name: " << name << ", pwd: " << pwd << std::endl;;
        return true;
    }

    /**
     * 重写基类UserServiceRpc的虚函数
    */
    virtual void Login(::google::protobuf::RpcController* controller,
                       const ::fixbug::LoginReuest* request,
                       ::fixbug::LoginResponse* response,
                       ::google::protobuf::Closure* done)
    {
        std::string name = request->name();
        std::string pwd = request->pwd();
        // 做本地业务
        bool login_result = Login(name, pwd);

        // 把响应写入
        fixbug::ResultCode* code = response->mutable_result();
        code->set_errcode(0);
        code->set_errmsg("");
        response->set_success(login_result);

        // 执行回调操作，由框架完成
        done->Run();
    }
};

int main(int argc, char* argv[]) {
    // 框架初始化
    MprpcApplication::Init(argc, argv);

    RpcProvider provider;
    provider.NotifyService(new UserService());

    provider.Run();

    return 0;
}