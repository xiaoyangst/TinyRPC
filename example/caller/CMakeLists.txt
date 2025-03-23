cmake_minimum_required(VERSION 3.16)
project(Caller)

#添加 debug 信息
set(CMAKE_BUILD_TYPE "Debug")

set(CALLER_SRC_LIST
        CallerService.cpp
        ${CMAKE_SOURCE_DIR}/example/user.pb.cc
)
add_executable(Caller ${CALLER_SRC_LIST})
target_link_libraries(Caller hv pthread protobuf::libprotobuf tinyrpc)

#target_include_directories(Caller PRIVATE ${CMAKE_SOURCE_DIR}/example
#        ${CMAKE_SOURCE_DIR}/src)

target_include_directories(Caller PRIVATE ${CMAKE_SOURCE_DIR}/example)
target_include_directories(Caller PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_include_directories(Caller PRIVATE ${CMAKE_SOURCE_DIR}/src/utils)
target_include_directories(Caller PRIVATE ${CMAKE_SOURCE_DIR}/src/rpc)
target_include_directories(Caller PRIVATE ${CMAKE_SOURCE_DIR}/src/proto)