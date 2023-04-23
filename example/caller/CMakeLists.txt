set(SRC_LIST calluserservice.cc ../user.pb.cc)
add_executable(consumer ${SRC_LIST})
target_link_libraries(consumer mprpc protobuf)

set(SRC_LIST_F callfriendservice.cc ../friend.pb.cc)
add_executable(f_consumer ${SRC_LIST_F})
target_link_libraries(f_consumer mprpc protobuf)