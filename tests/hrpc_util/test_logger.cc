#include "logger.h"

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
    for (int i = 0; i < 100; i++) {
        LOG_INFO("test info log print, i = %d", i);
    }
}

int main(int argc, char* argv[]) {
    
    Logger::GetInstance().Init("./config/log.conf");

    testPrintLog();
    // testCoreDump();

    return 0;
}