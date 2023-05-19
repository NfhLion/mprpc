#include "LogFile.h"

using namespace hrpc::util;

std::string hrpc::util::getDate() {
    // 获取当前的日期，然后取日志信息，写入相应的日志文件当中 a+
    time_t now = time(nullptr);
    tm *nowtm = localtime(&now);

    char time_name[128] = "\0";
    sprintf(time_name, "%d-%d-%d",
            nowtm->tm_year+1900,
            nowtm->tm_mon+1,
            nowtm->tm_mday);
    return time_name;
}

LogFile::LogFile(const std::string& outdir, 
            size_t rollSize,
            int flushInterval)
    : outdir_(outdir),
      rollSize_(rollSize*1024*1024),
      flushInterval_(flushInterval),
      appendCount_(0),
      count_(1),
      logDate_(getDate())
{
    rollFile();
}

LogFile::~LogFile()
{
}

void LogFile::append(const char* logline, int len) {
    std::string currentDate = getDate();
    if (logDate_.compare(currentDate) != 0) {
        logDate_ = currentDate;
        count_ = 1;
        appendCount_ = 0;
        rollFile();
    }

    file_->write(logline, len);
    appendCount_++;
    if (appendCount_ >= flushInterval_) {
        file_->flush();
    }

    if (file_->writeBytes() >= rollSize_) {
        count_++;
        appendCount_ = 0;
        rollFile();
    }
}

void LogFile::rollFile() {
    std::string filename = makeLogFilename(std::to_string(count_));
    file_.reset(new FileUtil(filename));
}

std::string LogFile::makeLogFilename(const std::string& suffix) {
    char file_name[128] = "\0";
    sprintf(file_name, "%s/%s_%s.log", 
            outdir_.c_str(),
            logDate_.c_str(),
            suffix.c_str());
    return file_name;
}