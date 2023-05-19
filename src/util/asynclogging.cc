#include "asynclogging.h"

#include <iostream>
#include <functional>

using namespace hrpc;
using namespace hrpc::util;

const int G_BUFFER_MAX_SIZE = 10;
const int G_BUFFERS_MAX_SIZE = 16;
const int G_ERROR_LOG_SIZE = 25;

AsyncLogging::AsyncLogging() 
    : m_exit(false),
      m_errorLog(false)
{
    currentBuffer_.reserve(G_BUFFER_MAX_SIZE);
    buffers_.reserve(G_BUFFERS_MAX_SIZE);
}

void AsyncLogging::start(const Config& logConfig) {
    // 启动专门的写日志线程
    writeLogThread_ = std::make_unique<std::thread>(std::bind(&AsyncLogging::threadHandler, this, logConfig));
}

AsyncLogging::~AsyncLogging() {
    m_exit = true;
    m_cond.notify_one();
    if (writeLogThread_->joinable()) {
        writeLogThread_->join();
    }
}

void AsyncLogging::threadHandler(const Config& logConfig) {
    std::string outdir = logConfig.Load("outdir");
    std::string s_rollSize = logConfig.Load("roll_size");
    if (s_rollSize == "") {
        output_ = std::make_unique<LogFile>(outdir);
    } else {
        size_t rollSize = std::stoul(s_rollSize);
        output_ = std::make_unique<LogFile>(outdir, rollSize);
    }

    std::vector<Buffer> writeBuffer;
    writeBuffer.reserve(16);
    writeBuffer.clear();
    for (;;)
    {
        if (m_exit) {
            break;
        }
        {
            std::unique_lock<std::mutex> lock(m_mutex);
            if (buffers_.empty()) {
                // 日志写入线程的第二种苏醒方式：每个三秒自动起来看一眼
                m_cond.wait_for(lock, std::chrono::seconds(3));
            }
            buffers_.push_back(std::move(currentBuffer_));
            Buffer nextBuffer;
            nextBuffer.reserve(G_BUFFER_MAX_SIZE);
            currentBuffer_ = std::move(nextBuffer);
            writeBuffer.swap(buffers_);
        }

        if (writeBuffer.size() > G_ERROR_LOG_SIZE) {
            m_errorLog = true;
            char buf[256];
            time_t now = time(nullptr);
            tm *nowtm = localtime(&now);
            sprintf(buf, "Dropped log messages at %d-%d-%d, %zd larger buffers\n",
                    nowtm->tm_year+1900, nowtm->tm_mon+1, nowtm->tm_mday,
                    writeBuffer.size()-2);
            fputs(buf, stderr);
            m_errorLogStr = buf;
            writeBuffer.erase(writeBuffer.begin()+2, writeBuffer.end());
        }
        
        for (int i = 0; i < writeBuffer.size(); ++i) {
            Buffer buf = writeBuffer[i];
            if (buf.empty()) {
                continue;
            }
            for (int j = 0; j < buf.size(); j++) {
                std::string msg = buf[j];
                msg.append("\n");
                output_->append(msg.c_str(), msg.length());
            }
        }
        if (m_errorLog) {
            m_errorLogStr.append("\n");
            output_->append(m_errorLogStr.c_str(), m_errorLogStr.length());
            m_errorLog = false;
        }
        
        writeBuffer.clear();
    }
}

// 前端线程往queue中写日志
void AsyncLogging::Log(const std::string& msg) {
    std::unique_lock<std::mutex> lock(m_mutex);
    if (currentBuffer_.size() < G_BUFFER_MAX_SIZE) {
        currentBuffer_.push_back(msg);
    } else {
        buffers_.push_back(std::move(currentBuffer_));
        Buffer nextBuffer;
        nextBuffer.reserve(G_BUFFER_MAX_SIZE);
        currentBuffer_ = std::move(nextBuffer);
        currentBuffer_.push_back(msg);

        m_cond.notify_one();
    }
}