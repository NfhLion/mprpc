set(HRPC_TEST_OUTPUT_DIR ${PROJECT_SOURCE_DIR}/bin)
set(HRPC_TEST_BUILD_RPATH "\$ORIGIN/../lib")

add_executable(test_threadpool test_threadpool.cc)
target_include_directories(test_threadpool PRIVATE ${PROJECT_SOURCE_DIR}/src/util)
target_link_libraries(test_threadpool PRIVATE hrpc_util pthread)
set_target_properties(test_threadpool PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${HRPC_TEST_OUTPUT_DIR}
    BUILD_RPATH ${HRPC_TEST_BUILD_RPATH}
)

add_executable(test_logger test_logger.cc)
target_include_directories(test_logger PRIVATE ${PROJECT_SOURCE_DIR}/src/util)
target_link_libraries(test_logger PRIVATE hrpc_util pthread)
set_target_properties(test_logger PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${HRPC_TEST_OUTPUT_DIR}
    BUILD_RPATH ${HRPC_TEST_BUILD_RPATH}
)
