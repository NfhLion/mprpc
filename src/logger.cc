#include "logger.h"

#include <iostream>
#include <chrono>
#include <time.h>

const int G_LOGQUEU_MAX_SIZE = 8;

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

static std::string MakeLogFilename() {
    // 获取当前的日期，然后取日志信息，写入相应的日志文件当中 a+
    time_t now = time(nullptr);
    tm *nowtm = localtime(&now);

    char file_name[128] = "\0";
    sprintf(file_name, "%d-%d-%d-log.txt", nowtm->tm_year+1900, nowtm->tm_mon+1, nowtm->tm_mday);
    return file_name;
}

std::string MakeLogMsgHeader(int level) {
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
    // 启动专门的写日志线程
    std::thread writeLogTask([&](){
        std::mutex mtx;
        for (;;)
        {
            // 日志写入线程的第二种苏醒方式：每个三秒自动起来看一眼
            std::unique_lock<std::mutex> lock(mtx);
            while (m_lckQue.empty()) {
                m_cond.wait_for(lock, std::chrono::seconds(3));
            }
            lock.unlock();
            
            std::string file_name = MakeLogFilename();
            FILE *pf = fopen(file_name.c_str(), "a+");
            if (pf == nullptr)
            {
                std::cout << "logger file : " << file_name << " open error!" << std::endl;
                exit(EXIT_FAILURE);
            }
            
            /**
             * 如果当前缓冲区大小 < G_LOGQUEU_MAX_SIZE，则正常去一条数据写入到文件
             * 否则将整个缓冲区数据全部写入到文件
            */
            if (m_lckQue.size() < G_LOGQUEU_MAX_SIZE) {
                std::string msg = m_lckQue.pop();
                msg.append("\n");
                fputs(msg.c_str(), pf);
            } else {
                std::queue<std::string> q;
                m_lckQue.swap(q);

                while (!q.empty()) {
                    std::string msg = q.front();
                    q.pop();
                    msg.append("\n");
                    fputs(msg.c_str(), pf);
                }
            }
            
            fclose(pf);
        }
    });
    writeLogTask.detach();
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
    m_lckQue.push(msg);
    m_cond.notify_one();
}