#include "logger.h"

#include <stdio.h>

using namespace hrpc::util;

int main(int argc, char* argv[]) {
    
    Logger::GetInstance().Init("./config/log.conf");

    for (int i = 0; i < 100; i++) {
        LOG_INFO("test info log print, i = %d", i);
    }

    return 0;
}