#ifndef HRPC_MPRPC_CHANNEL_H
#define HRPC_MPRPC_CHANNEL_H

#include <google/protobuf/service.h>

#include "zookeeperutil.h"

namespace hrpc
{

class MprpcChannel : public google::protobuf::RpcChannel {
public:

    void CallMethod(const google::protobuf::MethodDescriptor* method,
                          google::protobuf::RpcController* controller,
                          const google::protobuf::Message* request,
                          google::protobuf::Message* response, 
                          google::protobuf::Closure* done);
private:
    std::unique_ptr<ZkClient> m_zkClientPtr;
};

} // namespace hrpc

#endif // HRPC_MPRPC_CHANNEL_H