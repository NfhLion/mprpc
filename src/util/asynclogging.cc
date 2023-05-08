#include "asynclogging.h"

#include <iostream>

using namespace hrpc;
using namespace hrpc::util;

const int G_BUFFER_MAX_SIZE = 10;
const int G_BUFFERS_MAX_SIZE = 16;
const int G_ERROR_LOG_SIZE = 25;

static std::string MakeLogFilename(const char* outdir) {
    // 获取当前的日期，然后取日志信息，写入相应的日志文件当中 a+
    time_t now = time(nullptr);
    tm *nowtm = localtime(&now);

    char file_name[128] = "\0";
    sprintf(file_name, "%s/%d-%d-%d-log.txt", outdir, nowtm->tm_year+1900, nowtm->tm_mon+1, nowtm->tm_mday);
    return file_name;
}

AsyncLogging::AsyncLogging() 
    : m_exit(false),
      m_errorLog(false)
{
    currentBuffer_.reserve(G_BUFFER_MAX_SIZE);
    buffers_.reserve(G_BUFFERS_MAX_SIZE);
}

void AsyncLogging::Init() {
    // 启动专门的写日志线程
    std::thread writeLogTask([&](){
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

            
            std::string file_name = MakeLogFilename(m_outDir.c_str());
            FILE *pf = fopen(file_name.c_str(), "a+");
            if (pf == nullptr)
            {
                std::cout << "logger file : " << file_name << " open error!" << std::endl;
                exit(EXIT_FAILURE);
            }
            
            for (int i = 0; i < writeBuffer.size(); ++i) {
                Buffer buf = writeBuffer[i];
                if (buf.empty()) {
                    continue;
                }
                for (int j = 0; j < buf.size(); j++) {
                    std::string msg = buf[j];
                    msg.append("\n");
                    fputs(msg.c_str(), pf);
                }
            }
            if (m_errorLog) {
                m_errorLogStr.append("\n");
                fputs(m_errorLogStr.c_str(), pf);
                m_errorLog = false;
            }
            
            writeBuffer.clear();
            fclose(pf);
        }
    });
    writeLogTask.detach();
}

AsyncLogging::~AsyncLogging() {
    m_exit = true;
    m_cond.notify_one();
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

void AsyncLogging::SetOutDir(const std::string& outdir) {
    m_outDir = outdir;
}