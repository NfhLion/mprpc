set(HRPC_RPC_SOURCES
    rpc/mprpcapplication.cc
    rpc/mprpcchannel.cc
    rpc/mprpccontroller.cc
    rpc/rpcprovider.cc
    registry/zookeeperutil.cc
    proto/rpcheader.pb.cc
)

add_subdirectory(util)
add_subdirectory(net)

add_library(hrpc ${HRPC_LIBRARY_TYPE} ${HRPC_RPC_SOURCES})

target_include_directories(hrpc
    PUBLIC
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
    PRIVATE
        ${PROJECT_SOURCE_DIR}/src/proto
        ${PROJECT_SOURCE_DIR}/src/registry
        ${PROJECT_SOURCE_DIR}/src/rpc
        ${PROJECT_SOURCE_DIR}/src/net
        ${PROJECT_SOURCE_DIR}/src/util
)

target_link_libraries(hrpc
    PUBLIC
        hrpc_net
        hrpc_util
        protobuf::libprotobuf
        zookeeper_mt
        pthread
)

set_target_properties(hrpc PROPERTIES
    POSITION_INDEPENDENT_CODE ON
    VERSION ${PROJECT_VERSION}
    SOVERSION ${PROJECT_VERSION_MAJOR}
)

if (HRPC_BUILD_SHARED)
    target_compile_definitions(hrpc
        PUBLIC HRPC_SHARED
        PRIVATE HRPC_BUILDING_SHARED)

    set_target_properties(hrpc PROPERTIES
        CXX_VISIBILITY_PRESET hidden
        VISIBILITY_INLINES_HIDDEN YES
    )
endif()
