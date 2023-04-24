#include "mprpcapplication.h"
#include "logger.h"

#include <iostream>
#include <string>
#include <unistd.h>

MprpcConfig MprpcApplication::m_config;

void ShowArgHelp() {
    std::cout << "fomat: commid -i <configfile>" << std::endl; 
}

void MprpcApplication::Init(int argc, char **argv) {
    if (argc < 2) {
        ShowArgHelp();
        exit(EXIT_FAILURE);
    }

    int c = 0;
    std::string config_file;
    while ((c = getopt(argc, argv, "i:")) != -1) {
        switch (c) {
        case 'i':
            config_file = optarg;
            break;
        case '?':
            ShowArgHelp();
            exit(EXIT_FAILURE);
        case ':':
            ShowArgHelp();
            exit(EXIT_FAILURE);
        default:
            break;
        }
    }

    // 加载配置文件
    m_config.LoadConfigFile(config_file.c_str());

    // std::cout << "[ config info: ]" << std::endl;
    // std::cout << "rpcserverip: " << m_config.Load("rpcserverip") << std::endl;
    // std::cout << "rpcserverport: " << m_config.Load("rpcserverport") << std::endl;
    // std::cout << "zookeeperip: " << m_config.Load("zookeeperip") << std::endl;
    // std::cout << "zookeeperport: " << m_config.Load("zookeeperport") << std::endl;

    // 启动日志模块，设置日志等级
    Logger& logger =  Logger::GetInstance();
    std::string str = m_config.Load("loglevel");
    if (str != "") {
        logger.SetLogLevel(str);
    }
}

MprpcApplication& MprpcApplication::GetInstance() {
    static MprpcApplication app;
    return app;
}

MprpcConfig& MprpcApplication::GetConfig() {
    return m_config;
}