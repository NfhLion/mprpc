#include "test.pb.h"
#include <iostream>

using namespace fixbug;

void testLoginReuest() {
    // 封装了login请求对象的数据
    LoginReuest req;
    req.set_name("zhang san");
    req.set_pwd("123456");

    // 对象数据序列化 -> char*
    std::string send_str;
    if (req.SerializeToString(&send_str)) {
        std::cout << send_str << std::endl;
    }

    // 从send_str反序列化一个login请求对象
    LoginReuest reqb;
    if (reqb.ParseFromString(send_str)) {
        std::cout << reqb.name() << std::endl;
        std::cout << reqb.pwd() << std::endl;
    }
}

void testLink() {
    GetFriendListsResponse rsp;
    ResultCode* rc = rsp.mutable_result();
    rc->set_errcode(0);

    for (int i = 0; i < 10; ++i) {
        user* user1 = rsp.add_friend_lists();
        user1->set_name("zhang san");
        user1->set_age(21 + i);
        user1->set_sex(user::MAN);
    }

    std::cout << rsp.friend_lists_size() << std::endl;
}

int main() {

    // LoginResponse rsp;
    // ResultCode* rc = rsp.mutable_result();
    // rc->set_errcode(1);
    // rc->set_errmsg("Login Failed");
     
    

    return 0;
}