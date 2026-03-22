#ifndef HRPC_MPRPC_APPLICATION_H
#define HRPC_MPRPC_APPLICATION_H

#include "config.h"

using namespace hrpc::util;
namespace hrpc
{
// mprpc的基础类
class MprpcApplication {
public:
    static void Init(int argc, char **argv);

    static MprpcApplication& GetInstance();
    static Config& GetConfig();

private:
    static Config m_config;
    MprpcApplication() {};
    MprpcApplication(const MprpcApplication&) = delete;
    MprpcApplication(MprpcApplication&&) = delete;
};

} // namespace hrpc

#endif // HRPC_MPRPC_APPLICATION_H