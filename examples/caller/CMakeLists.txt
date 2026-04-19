add_executable(consumer
    calluserservice.cc
    ../publictype.pb.cc
    ../user.pb.cc
)
target_include_directories(consumer PRIVATE ${PROJECT_SOURCE_DIR}/examples)
target_link_libraries(consumer PRIVATE hrpc protobuf::libprotobuf)

add_executable(f_consumer
    callfriendservice.cc
    ../publictype.pb.cc
    ../friend.pb.cc
)
target_include_directories(f_consumer PRIVATE ${PROJECT_SOURCE_DIR}/examples)
target_link_libraries(f_consumer PRIVATE hrpc protobuf::libprotobuf)

add_executable(chat_consumer
    callchatservice.cc
    ../publictype.pb.cc
    ../user.pb.cc
    ../friend.pb.cc
)
target_include_directories(chat_consumer PRIVATE ${PROJECT_SOURCE_DIR}/examples)
target_link_libraries(chat_consumer PRIVATE hrpc protobuf::libprotobuf)
