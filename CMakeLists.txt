
cmake_minimum_required(VERSION 3.5)

project(testcpp)

enable_language(ASM)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-mavx2 -mavx512f")
link_directories(${PROJECT_SOURCE_DIR})

add_executable(testcpp main.cpp)

target_include_directories(testcpp PRIVATE "/home/liuyujun/opensource/fmt/include")

target_link_libraries(testcpp)