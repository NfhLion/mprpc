#pragma once

#include "lockqueue.h"

#include <string>
#include <thread>
#include <condition_variable>

class AsyncLogging {
public:
    AsyncLogging();
    ~AsyncLogging();
    void Log(const std::string& msg);

    void SetOutDir(const std::string& outdir);
    void Init();

private:
    bool m_exit;
    LockQueue<std::string> m_lckQue;
    std::condition_variable m_cond;

    std::string m_outDir;

    AsyncLogging(const AsyncLogging&) = delete;
    AsyncLogging(AsyncLogging&&) = delete;
};