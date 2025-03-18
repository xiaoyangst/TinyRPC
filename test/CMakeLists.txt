cmake_minimum_required(VERSION 3.16)
project(Test)


add_executable(ConfigTest ${CMAKE_SOURCE_DIR}/src/utils/Config.cpp ConfigTest.cpp)
target_link_libraries(ConfigTest PRIVATE GTest::GTest GTest::Main pthread)
target_include_directories(ConfigTest PRIVATE ${CMAKE_SOURCE_DIR}/src)

# 注册测试
include(GoogleTest)
gtest_discover_tests(ConfigTest)