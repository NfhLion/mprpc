set(SRC_LIST SocketsOps.cc
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

add_library(hrpc_net ${SRC_LIST})
# target_link_libraries(mprpc muduo_net muduo_base pthread zookeeper_mt)