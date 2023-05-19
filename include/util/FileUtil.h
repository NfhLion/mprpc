#pragma once

#include <string>

#include "noncopyable.h"

namespace hrpc
{
namespace util
{

class FileUtil : public noncopyable{
public:

    FileUtil(const std::string& filename);

    ~FileUtil();

    void write(const char* logline, size_t len);

    void read() {}

    void flush();

    size_t writeBytes() const { 
        return writeBytes_;
    }

private:

    size_t writeData(const char* logline, size_t len);

    FILE* fp_;
    char buffer_[64*1024];
    size_t writeBytes_;
};

} // namespace util
} // namespace hrpc