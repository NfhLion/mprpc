cmake_minimum_required(VERSION 3.0)
project(hrpc_util_test)

# 设置项目可执行文件输出的路径
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

# 设置项目编译头文件搜索路径 -I
include_directories(${PROJECT_SOURCE_DIR}/../../lib/include/util)

link_directories(${PROJECT_SOURCE_DIR}/../../lib)

# set(SRC_LIST test_threadpool.cc)
# add_executable(test_threadpool ${SRC_LIST})
# target_link_libraries(test_threadpool hrpc_util pthread)

set(SRC_LIST test_logger.cc)
add_executable(test_logger ${SRC_LIST})
target_link_libraries(test_logger hrpc_util pthread)