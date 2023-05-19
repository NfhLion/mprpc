#pragma once

#include <string>
#include <memory>

#include "noncopyable.h"
#include "FileUtil.h"

namespace hrpc
{
namespace util
{

const size_t DEFAULT_ROLL_SIZE = 8;   // 8M
const int DEFAULT_FLUSH_INTERVAL = 3;

std::string getDate();

class LogFile : public noncopyable{
public:
    LogFile(const std::string& outdir, 
            size_t rollSize = DEFAULT_ROLL_SIZE*1024*1024,
            int flushInterval = DEFAULT_FLUSH_INTERVAL);
    ~LogFile();

    void append(const char* logline, int len);
    void rollFile();

private:
    std::string makeLogFilename(const std::string& suffix);


    std::unique_ptr<FileUtil> file_;

    std::string logDate_;   // 当前日志的日期
    std::string outdir_;
    size_t rollSize_;       // 单个日志文件大小
    int flushInterval_;     // flush间隔
    int appendCount_;       // 记录append几次
    int count_;             // 记录单日回滚文件次数
};

} // namespace util
} // namespace hrpc