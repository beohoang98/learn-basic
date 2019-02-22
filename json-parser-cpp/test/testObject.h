//
// Created by noobcoder on 22/02/2019.
//

#ifndef JSON_PARSER_CPP_TESTOBJECT_H
#define JSON_PARSER_CPP_TESTOBJECT_H

#include <gtest/gtest.h>
#include "../src/BSON.h"

TEST(test_object, test_object_1) {
    const wchar_t * buf = LR"({"name":"Hello World"})";
    BSON bson;
    size_t next = BSON::parseJSON(buf, wcslen(buf), 0, bson);
    EXPECT_NE(0, next);
}

TEST(test_object, test_object_2) {
    const wchar_t * buf = LR"({"name":"Hello World"})";
    BSON bson;
    size_t next = BSON::parseJSON(buf, wcslen(buf), 0, bson);
    wstring value = bson.getString(L"name");
    EXPECT_EQ(L"Hello World", value);
}

TEST(test_object, test_object_3) {
    const wchar_t * buf = LR"({"me":{"name":"An"}, "girlfriend": { "name" : "Someone"}})";
    BSON bson;
    size_t next = BSON::parseJSON(buf, wcslen(buf), 0, bson);
    EXPECT_NE(0, next);

    BSON me = bson.get(L"me");
    EXPECT_EQ(L"An", me.getString(L"name"));
    EXPECT_EQ(L"Someone", bson.get(L"girlfriend").getString(L"name"));
}

TEST(test_object, test_object_4) {
    const wchar_t * buf = LR"({"me":{"name":"An", "age": 21}, "girlfriend": { "name" : "Someone", "age" : 21}})";
    BSON bson;
    size_t next = BSON::parseJSON(buf, wcslen(buf), 0, bson);
    EXPECT_NE(0, next);

    BSON me = bson.get(L"me");
    EXPECT_EQ(21, me.getInt(L"age"));
    EXPECT_EQ(21, bson.get(L"girlfriend").getInt(L"age"));
}

TEST(test_object, test_object_fail_1) {
    const wchar_t * buf = LR"({"name":"Hello World"})";
    BSON bson;
    size_t next = BSON::parseJSON(buf, wcslen(buf), 0, bson);

    EXPECT_THROW(bson.get(L"name"), bad_alloc);
}

#endif //JSON_PARSER_CPP_TESTOBJECT_H
