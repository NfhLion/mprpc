set(SRC_LIST userservice.cc ../user.pb.cc)
add_executable(provider ${SRC_LIST})
target_link_libraries(provider mprpc protobuf)

set(SRC_LIST_F friendservice.cc ../friend.pb.cc)
add_executable(f_provider ${SRC_LIST_F})
target_link_libraries(f_provider mprpc protobuf)