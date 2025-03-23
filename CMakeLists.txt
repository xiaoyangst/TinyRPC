cmake_minimum_required(VERSION 3.16)
project(TinyRpc)

find_package(GTest REQUIRED)
find_package(Protobuf REQUIRED)

set(CMAKE_CXX_STANDARD 17)
# 设置项目可执行文件输出的路径
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
# 设置项目库文件输出的路径
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)

add_subdirectory(src)
add_subdirectory(example)
add_subdirectory(test)
add_executable(TinyRpc main.cpp)
