cmake_minimum_required(VERSION 3.0)
project(hrpc_net_test)

# 设置项目可执行文件输出的路径
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR})

# 设置项目编译头文件搜索路径 -I
include_directories(${PROJECT_SOURCE_DIR}/../../lib/include/util)
include_directories(${PROJECT_SOURCE_DIR}/../../lib/include/net)

link_directories(${PROJECT_SOURCE_DIR}/../../lib)

set(SRC_LIST echo_server.cc)
add_executable(echo_server ${SRC_LIST})
target_link_libraries(echo_server hrpc_net)