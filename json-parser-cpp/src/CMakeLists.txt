cmake_minimum_required(VERSION 3.13)
project(json_parser_cpp)

set(CMAKE_CXX_STANDARD 14)

add_executable(json_parser_cpp BSON.cpp BSON.h Any.h)