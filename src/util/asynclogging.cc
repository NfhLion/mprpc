#include "asynclogging.h"

#include <iostream>

using namespace hrpc;
using namespace hrpc::util;

const int G_LOGQUEU_MAX_SIZE = 8;

static std::string MakeLogFilename(const char* outdir) {
    // 获取当前的日期，然后取日志信息，写入相应的日志文件当中 a+
    time_t now = time(nullptr);
    tm *nowtm = localtime(&now);

    char file_name[128] = "\0";
    sprintf(file_name, "%s/%d-%d-%d-log.txt", outdir, nowtm->tm_year+1900, nowtm->tm_mon+1, nowtm->tm_mday);
    return file_name;
}

AsyncLogging::AsyncLogging() {
    m_exit = false;
}

void AsyncLogging::Init() {
    // 启动专门的写日志线程
    std::thread writeLogTask([&](){
        std::mutex mtx;
        for (;;)
        {
            // 日志写入线程的第二种苏醒方式：每个三秒自动起来看一眼
            std::unique_lock<std::mutex> lock(mtx);
            while (m_lckQue.empty()) {
                m_cond.wait_for(lock, std::chrono::seconds(3));
                if (m_exit == true) {
                    break;
                }
            }
            lock.unlock();
            if (m_exit == true) {
                break;
            }
            
            std::string file_name = MakeLogFilename(m_outDir.c_str());
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

AsyncLogging::~AsyncLogging() {
    m_exit = true;
    m_cond.notify_one();
}

// 前端线程往queue中写日志
void AsyncLogging::Log(const std::string& msg) {
    m_lckQue.push(msg);
    m_cond.notify_one();
}

void AsyncLogging::SetOutDir(const std::string& outdir) {
    m_outDir = outdir;
}