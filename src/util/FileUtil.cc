#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>

#include "FileUtil.h"

using namespace hrpc::util;

FileUtil::FileUtil(const std::string& filename) 
    : fp_(::fopen(filename.c_str(), "ae")),  // 'e' for O_CLOEXEC
    writeBytes_(0)
{
    assert(fp_);
    ::setbuffer(fp_, buffer_, sizeof(buffer_));
}

FileUtil::~FileUtil() {
    ::fclose(fp_);
}

void FileUtil::write(const char* logline, size_t len) {
    size_t n = writeData(logline, len);
    size_t remain = len - n;
    while (remain > 0) {
        size_t x = writeData(logline + n, remain);
        if (x == 0) {
            int err = ferror(fp_);
            if (err) {
                fprintf(stderr, "FileUtil::write() failed\n");
            }
            break;
        }
        n += x;
        remain = len - n; // remain -= x
    }
    writeBytes_ += len;
}

size_t FileUtil::FileUtil::writeData(const char* logline, size_t len)
{
  // #undef fwrite_unlocked
  return ::fwrite_unlocked(logline, 1, len, fp_);
}

void FileUtil::flush() {
  ::fflush(fp_);
}