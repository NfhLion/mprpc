add_executable(provider
    userservice.cc
    ../publictype.pb.cc
    ../user.pb.cc
)
target_include_directories(provider PRIVATE ${PROJECT_SOURCE_DIR}/examples ${PROJECT_SOURCE_DIR}/src/util)
target_link_libraries(provider PRIVATE hrpc protobuf::libprotobuf)

add_executable(f_provider
    friendservice.cc
    ../publictype.pb.cc
    ../friend.pb.cc
)
target_include_directories(f_provider PRIVATE ${PROJECT_SOURCE_DIR}/examples)
target_link_libraries(f_provider PRIVATE hrpc protobuf::libprotobuf)

add_executable(chat_provider
    chatservice.cc
    ../publictype.pb.cc
    ../user.pb.cc
    ../friend.pb.cc
)
target_include_directories(chat_provider PRIVATE ${PROJECT_SOURCE_DIR}/examples ${PROJECT_SOURCE_DIR}/src/util)
target_link_libraries(chat_provider PRIVATE hrpc protobuf::libprotobuf)
