#include "logger.h"

#include <filesystem>
#include <stdio.h>

using namespace hrpc::util;

// 获取core时落盘日志的方法：https://www.yuque.com/linuxer/xngi03/pfq9m1?
void testCoreDump() {
    for (int i = 0; i < 1000000; i++) {
        LOG_INFO("test info log print, i = %d", i);
        int* p = nullptr;
        if (i == 500000) {
            *p = 0x1234;
        }
    }
}

void testPrintLog() {
    for (int i = 0; i < 1000000; i++) {
        LOG_INFO("test info log print, i = %d", i);
    }
}

int main(int argc, char* argv[]) {
    std::filesystem::path exe_path = std::filesystem::absolute(argv[0]);
    std::filesystem::path exe_dir = exe_path.parent_path();
    std::filesystem::current_path(exe_dir);
    std::filesystem::create_directories(exe_dir / "log");
    std::filesystem::path config_path = exe_dir / "config" / "log.conf";
    Logger::GetInstance().Init(config_path.string());

    testPrintLog();
    // testCoreDump();

    return 0;
}
