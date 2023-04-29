#pragma once

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