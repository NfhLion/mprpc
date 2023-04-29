#include "logger.h"

#include <iostream>
#include <chrono>
#include <time.h>

using namespace hrpc;
using namespace hrpc::util;

static std::unordered_map<int, std::string> g_logLevelMap = 
{
    {TRACE, "TRACE"},
    {DEBUG, "DEBUG"},
    {INFO, "INFO"},
    {WARN, "WARN"},
    {ERROR, "ERROR"},
    {FATAL, "FATAL"}
};

static std::unordered_map<std::string, int> g_logLevelStrMap = 
{
    {"TRACE", TRACE},
    {"DEBUG", DEBUG},
    {"INFO", INFO},
    {"WARN", WARN},
    {"ERROR", ERROR},
    {"FATAL", FATAL}
};

std::string hrpc::util::MakeLogMsgHeader(int level) {
    time_t now = time(nullptr);
    tm *nowtm = localtime(&now);
    char time_buf[128] = "\0";
    sprintf(time_buf, "%d-%d-%d %d:%d:%d [%s]",
            nowtm->tm_year+1900,
            nowtm->tm_mon+1,
            nowtm->tm_mday,
            nowtm->tm_hour, 
            nowtm->tm_min, 
            nowtm->tm_sec,
            g_logLevelMap.at(level).c_str());
    return time_buf;
}

Logger& Logger::GetInstance() {
    static Logger logger;
    return logger;
}

Logger::Logger() {
    // 默认日志等级是INFO
    m_loglevel = INFO;
    m_tag = "DEFAULT";
    m_asyncLogging = nullptr;
}

Logger::~Logger() {
    if (m_asyncLogging != nullptr) {
        delete m_asyncLogging;
    }
}

void Logger::Init(const std::string& path) {
    m_logConfig.LoadConfigFile(path.c_str());

    m_tag = m_logConfig.Load("logtag");
    std::string out_dir = m_logConfig.Load("outdir");
    if (out_dir != "") {
        m_asyncLogging = new AsyncLogging();
        m_asyncLogging->SetOutDir(out_dir);
        m_asyncLogging->Init();
    }
}

void Logger::SetLogLevel(LogLevel level) {
    m_loglevel = level;
}

void Logger::SetLogLevel(const std::string& level) {
    auto it = g_logLevelStrMap.find(level);
    if (it == g_logLevelStrMap.end()) {
        std::cout << "< set log level is error! > level: "
                  << "[ INFO|ERROR ]" << std::endl;
        exit(EXIT_FAILURE);
    }
}

// 前端线程往queue中写日志
void Logger::Log(const std::string& msg) {
    // 输出到标准输出
    printf("%s\n", msg.c_str());
    if (m_asyncLogging != nullptr) {
        m_asyncLogging->Log(msg);
    }
}