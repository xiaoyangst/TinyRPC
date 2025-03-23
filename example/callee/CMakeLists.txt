cmake_minimum_required(VERSION 3.16)
project(Callee)

set(CMAKE_BUILD_TYPE "Debug")

set(CALLEE_SRC_LIST
        UserService.cpp
        ${CMAKE_SOURCE_DIR}/example/user.pb.cc
)

add_executable(Callee ${CALLEE_SRC_LIST})
target_link_libraries(Callee hv pthread protobuf::libprotobuf tinyrpc)

#target_include_directories(Callee PRIVATE ${CMAKE_SOURCE_DIR}/example
#        ${CMAKE_SOURCE_DIR}/src)

target_include_directories(Callee PRIVATE ${CMAKE_SOURCE_DIR}/example)
target_include_directories(Callee PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_include_directories(Callee PRIVATE ${CMAKE_SOURCE_DIR}/src/utils)
target_include_directories(Callee PRIVATE ${CMAKE_SOURCE_DIR}/src/rpc)
target_include_directories(Callee PRIVATE ${CMAKE_SOURCE_DIR}/src/proto)