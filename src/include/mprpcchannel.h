#pragma once

#include <google/protobuf/service.h>

class MprpcChannel : public google::protobuf::RpcChannel {
public:

    void CallMethod(const google::protobuf::MethodDescriptor* method,
                          google::protobuf::RpcController* controller,
                          const google::protobuf::Message* request,
                          google::protobuf::Message* response, 
                          google::protobuf::Closure* done);
private:

};