#pragma once

#include "LogFile.h"
#include "config.h"

#include <string>
#include <thread>
#include <condition_variable>
#include <vector>

namespace hrpc
{
namespace util
{

class AsyncLogging {
public:
    AsyncLogging();
    ~AsyncLogging();
    void Log(const std::string& msg);

    void start(const Config& logConfig);

private:
    void threadHandler(const Config& logConfig);

    using Buffer = std::vector<std::string>;

    bool m_exit;
    bool m_errorLog;
    std::string m_errorLogStr;

    Buffer currentBuffer_;
    std::vector<Buffer> buffers_;

    std::unique_ptr<std::thread> writeLogThread_;
    std::mutex m_mutex;
    std::condition_variable m_cond;

    std::string m_outDir;
    std::unique_ptr<LogFile> output_;

    AsyncLogging(const AsyncLogging&) = delete;
    AsyncLogging(AsyncLogging&&) = delete;
};

} // namespace util
} // namespace hrpc