set(HRPC_TEST_OUTPUT_DIR ${PROJECT_SOURCE_DIR}/bin)
set(HRPC_TEST_BUILD_RPATH "\$ORIGIN/../lib")

add_executable(echo_server echo_server.cc)
target_include_directories(echo_server PRIVATE ${PROJECT_SOURCE_DIR}/src/net ${PROJECT_SOURCE_DIR}/src/util)
target_link_libraries(echo_server PRIVATE hrpc_net)
set_target_properties(echo_server PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${HRPC_TEST_OUTPUT_DIR}
    BUILD_RPATH ${HRPC_TEST_BUILD_RPATH}
)
