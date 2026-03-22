#ifndef HRPC_MPRPC_CONTROLLER_H
#define HRPC_MPRPC_CONTROLLER_H

#include <google/protobuf/service.h>
#include <string>

namespace hrpc
{

class MprpcController : public google::protobuf::RpcController {
public:
    MprpcController();
    ~MprpcController(){}

    void Reset();
    bool Failed() const;
    std::string ErrorText() const;
    void SetFailed(const std::string& reason);

    void StartCancel();
    bool IsCanceled() const;
    void NotifyOnCancel(google::protobuf::Closure* callback);

private:
    bool m_failed;
    std::string m_errText;
};

} // namespace hrpc

#endif // HRPC_MPRPC_CONTROLLER_H