set(HRPC_NET_SOURCES
    SocketsOps.cc
    InetAddress.cc
    Socket.cc
    EventLoop.cc
    Channel.cc
    Poller.cc
    Buffer.cc
    Acceptor.cc
    TcpConnection.cc
    TcpServer.cc
)

add_library(hrpc_net ${HRPC_LIBRARY_TYPE} ${HRPC_NET_SOURCES})

target_include_directories(hrpc_net
    PUBLIC
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/src/net>
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/src/util>
)

target_link_libraries(hrpc_net
    PUBLIC
        hrpc_util
        pthread
)

set_target_properties(hrpc_net PROPERTIES
    POSITION_INDEPENDENT_CODE ON
    VERSION ${PROJECT_VERSION}
    SOVERSION ${PROJECT_VERSION_MAJOR}
)
