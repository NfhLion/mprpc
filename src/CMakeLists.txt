set(SRC_LIST mprpcapplication.cc
             rpcheader.pb.cc
             rpcprovider.cc
             mprpcconfig.cc
             mprpccontroller.cc
             mprpcchannel.cc
             logger.cc)

add_library(mprpc ${SRC_LIST})
target_link_libraries(mprpc muduo_net muduo_base pthread)