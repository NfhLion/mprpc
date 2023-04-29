set(SRC_LIST mprpcapplication.cc
             rpcheader.pb.cc
             rpcprovider.cc
             mprpccontroller.cc
             mprpcchannel.cc
             zookeeperutil.cc)

add_library(hrpc ${SRC_LIST})
target_link_libraries(hrpc hrpc_net hrpc_util pthread zookeeper_mt)

add_subdirectory(net)
add_subdirectory(util)