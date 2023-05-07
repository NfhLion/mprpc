#pragma once

#include "config.h"
#include "asynclogging.h"

#include <time.h>
#include <string>
#include <unordered_map>

namespace hrpc
{
namespace util
{

enum LogLevel
{
    TRACE,  // 调试，指出比DEBUG粒度更细的一些信息事件（开发过程中使用）
    DEBUG,  // 调试，指出细粒度信息事件对调试应用程序是非常有帮助的。（开发过程中使用）
    INFO,   // 信息，表明消息在粗粒度级别上突出强调应用程序的运行过程。
    WARN,   // 警告，系统能正常运行，但可能会出现潜在错误的情形。
    ERROR,  // 错误，指出虽然发生错误事件，但仍然不影响系统的继续运行
    FATAL   // 崩溃，指出每个严重的错误事件将会导致应用程序的退出。
};

std::string MakeLogMsgHeader(int level);
class Logger;

// 定义宏 LOG_INFO("xxx %d %s", 20, "xxxx")
#define LOG_PRINT(logmsgformat, ...) \
    do \
    {  \
        Logger &logger = Logger::GetInstance(); \
        char pre[1024] = {0};\
        snprintf(pre, 1024, "[%s] %s [%s %d] ", logger.GetTag().c_str(), MakeLogMsgHeader(logger.GetLogLevel()).c_str(),\
                                    __FILE__, __LINE__); \
        char info[1024] = {0}; \
        snprintf(info, 1024, logmsgformat, ##__VA_ARGS__); \
        std::string log_info = std::string(pre) + std::string(info); \
        logger.Log(log_info.c_str()); \
    } while(0); \

#define LOG_TRACE(logmsgformat, ...) \
    do \
    {  \
        if (TRACE >= Logger::GetInstance().GetLogLevel()) { \
            LOG_PRINT(logmsgformat, ##__VA_ARGS__); \
        }   \
    } while(0); \

#define LOG_DEBUG(logmsgformat, ...) \
    do \
    {  \
        if (DEBUG >= Logger::GetInstance().GetLogLevel()) { \
            LOG_PRINT(logmsgformat, ##__VA_ARGS__); \
        }   \
    } while(0); \

#define LOG_INFO(logmsgformat, ...) \
    do \
    {  \
        if (INFO >= Logger::GetInstance().GetLogLevel()) { \
            LOG_PRINT(logmsgformat, ##__VA_ARGS__); \
        }   \
    } while(0); \

#define LOG_WARN(logmsgformat, ...) \
    do \
    {  \
        if (WARN >= Logger::GetInstance().GetLogLevel()) { \
            LOG_PRINT(logmsgformat, ##__VA_ARGS__); \
        }   \
    } while(0); \

#define LOG_ERROR(logmsgformat, ...) \
    do \
    {  \
        if (ERROR >= Logger::GetInstance().GetLogLevel()) { \
            LOG_PRINT(logmsgformat, ##__VA_ARGS__); \
        }   \
    } while(0); \

#define LOG_FATAL(logmsgformat, ...) \
    do \
    {  \
        if (FATAL >= Logger::GetInstance().GetLogLevel()) { \
            LOG_PRINT(logmsgformat, ##__VA_ARGS__); \
        }   \
    } while(0); \

class Logger {
public:
    void Init(const std::string& path);
    void SetLogLevel(LogLevel level);
    void SetLogLevel(const std::string& level);
    int GetLogLevel() const { return m_loglevel; }
    const std::string& GetTag() const { return m_tag; }
    void Log(const std::string& msg);

    static Logger& GetInstance();

private:
    int m_loglevel;
    std::string m_tag;
    Config m_logConfig;

    AsyncLogging* m_asyncLogging;

    Logger();
    ~Logger();
    Logger(const Logger&) = delete;
    Logger(Logger&&) = delete;
};

} // namespace util
} // namespace hrpc