cmake_minimum_required(VERSION 3.10)

PROJECT(webserver_cpp)
set(CMAKE_CXX_STANDARD 14)

set(ENV{WORKING_DIR} "${CMAKE_CURRENT_SOURCE_DIR}")
set(SOURCE main.cpp controller.h)

set(EXECUTABLE_OUTPUT_PATH "${CMAKE_CURRENT_SOURCE_DIR}/web")
add_subdirectory(http)

include_directories(http)

add_executable(index ${SOURCE})
target_link_libraries(index httpserver)
