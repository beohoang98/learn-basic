cmake_minimum_required(VERSION 3.13)
project(json_parser_cpp)
set(CMAKE_CXX_STANDARD 14)
set(SOURCE testParseString.h main.cpp ../src/BSON.cpp ../src/BSON.h testNumber.h testObject.h)
set(gtest_SOURCE_DIR ../googletest/googletest)

find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

add_executable(json_parser_test ${SOURCE})

target_link_libraries(json_parser_test ${GTEST_LIBRARIES} ${GTEST_MAIN_LIBRARIES} pthread)
