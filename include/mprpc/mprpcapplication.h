#ifndef HRPC_MPRPC_APPLICATION_H
#define HRPC_MPRPC_APPLICATION_H

#include "mprpc/export.h"

namespace hrpc
{
namespace util
{
class Config;
}

// mprpc的基础类
class HRPC_API MprpcApplication {
public:
    static void Init(int argc, char **argv);

    static MprpcApplication& GetInstance();
    static util::Config& GetConfig();

private:
    static util::Config m_config;
    MprpcApplication() {};
    MprpcApplication(const MprpcApplication&) = delete;
    MprpcApplication(MprpcApplication&&) = delete;
};

} // namespace hrpc

#endif // HRPC_MPRPC_APPLICATION_H
