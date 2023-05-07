#pragma once

#include "lockqueue.h"

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

    void SetOutDir(const std::string& outdir);
    void Init();

private:
    using Buffer = std::vector<std::string>;

    bool m_exit;
    bool m_errorLog;
    std::string m_errorLogStr;

    Buffer currentBuffer_;
    std::vector<Buffer> buffers_;

    std::mutex m_mutex;
    std::condition_variable m_cond;

    std::string m_outDir;

    AsyncLogging(const AsyncLogging&) = delete;
    AsyncLogging(AsyncLogging&&) = delete;
};

} // namespace util
} // namespace hrpc