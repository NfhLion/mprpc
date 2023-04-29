set(SRC_LIST userservice.cc ../publictype.pb.cc ../user.pb.cc)
add_executable(provider ${SRC_LIST})
target_link_libraries(provider hrpc protobuf)

set(SRC_LIST_F friendservice.cc ../publictype.pb.cc ../friend.pb.cc)
add_executable(f_provider ${SRC_LIST_F})
target_link_libraries(f_provider hrpc protobuf)

set(SRC_LIST_C 
    chatservice.cc
    ../publictype.pb.cc
    ../user.pb.cc 
    ../friend.pb.cc)
add_executable(chat_provider ${SRC_LIST_C})
target_link_libraries(chat_provider hrpc protobuf)