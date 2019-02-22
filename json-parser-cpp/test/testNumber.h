//
// Created by noobcoder on 22/02/2019.
//

#ifndef JSON_PARSER_CPP_TESTNUMBER_H
#define JSON_PARSER_CPP_TESTNUMBER_H

#include <gtest/gtest.h>
#include "../src/BSON.h"

TEST(json_parser_test_number, testNumber1) {
    const wchar_t * buf = LR"( -100)";
    int result;
    BSON::parseNumber(buf, wcslen(buf), 0, result);
    EXPECT_EQ(-100, result);
}

TEST(json_parser_test_number, testNumber2) {
    const wchar_t * buf = LR"(102220,)";
    int result;
    BSON::parseNumber(buf, wcslen(buf), 0, result);
    EXPECT_EQ(102220, result);
}

TEST(json_parser_test_number, testNumber3) {
    const wchar_t * buf = LR"(00512], asd)";
    int result;
    BSON::parseNumber(buf, wcslen(buf), 0, result);
    EXPECT_EQ(512, result);
}

TEST(json_parser_test_number, testNumberFail) {
    const wchar_t * buf = LR"(a)";
    int result;
    size_t next = BSON::parseNumber(buf, wcslen(buf), 0, result);
    EXPECT_EQ(0, next);
}

TEST(json_parser_test_number, testNumberFail2) {
    const wchar_t * buf = LR"(--022)";
    int result;
    size_t next = BSON::parseNumber(buf, wcslen(buf), 0, result);
    EXPECT_EQ(0, next);
}

#endif //JSON_PARSER_CPP_TESTNUMBER_H
