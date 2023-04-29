set(SRC_LIST mprpcapplication.cc
             rpcheader.pb.cc
             rpcprovider.cc
             mprpcconfig.cc
             mprpccontroller.cc
             mprpcchannel.cc
             asynclogging.cc
             logger.cc
             zookeeperutil.cc)

add_library(mprpc ${SRC_LIST})
target_link_libraries(mprpc hrpc_net pthread zookeeper_mt)

add_subdirectory(net)
#add_subdirectory(util)