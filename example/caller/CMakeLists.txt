set(SRC_LIST calluserservice.cc ../publictype.pb.cc ../user.pb.cc)
add_executable(consumer ${SRC_LIST})
target_link_libraries(consumer mprpc protobuf)

set(SRC_LIST_F callfriendservice.cc ../publictype.pb.cc ../friend.pb.cc)
add_executable(f_consumer ${SRC_LIST_F})
target_link_libraries(f_consumer mprpc protobuf)

set(SRC_LIST_C 
    callchatservice.cc
    ../publictype.pb.cc
    ../user.pb.cc 
    ../friend.pb.cc)
add_executable(chat_consumer ${SRC_LIST_C})
target_link_libraries(chat_consumer mprpc protobuf)