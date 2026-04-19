#ifndef HRPC_MPRPC_CHANNEL_H
#define HRPC_MPRPC_CHANNEL_H

#include <google/protobuf/service.h>

#include "mprpc/export.h"

namespace hrpc
{
class ZkClient;

class HRPC_API MprpcChannel : public google::protobuf::RpcChannel {
public:
    MprpcChannel();
    ~MprpcChannel();

    void CallMethod(const google::protobuf::MethodDescriptor* method,
                          google::protobuf::RpcController* controller,
                          const google::protobuf::Message* request,
                          google::protobuf::Message* response, 
                          google::protobuf::Closure* done);
private:
    ZkClient* m_zkClientPtr;
};

} // namespace hrpc

#endif // HRPC_MPRPC_CHANNEL_H
