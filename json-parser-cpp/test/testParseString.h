//
// Created by noobcoder on 22/02/2019.
//
#include "gtest/gtest.h"
#include "../src/BSON.h"

class TestPareString : public testing::Test
{
protected:
    void SetUp() override {
        Test::SetUp();
    }

    void TearDown() override {
        Test::TearDown();
    }

public:
    TestPareString() : Test() {
        buffer = LR"(    "Thanh pho\n\tbe den the thoi\n"  )";
        length = wcslen(buffer);
    }

    size_t length;
    const wchar_t * buffer;
};

TEST_F(TestPareString, testParseString) {
    wstring str;
    size_t next = BSON::parseString(buffer, length, 0, str);
    EXPECT_EQ(L"Thanh pho\n\tbe den the thoi\n", str);
}

TEST_F(TestPareString, testParseStringIndex) {
    wstring str;
    size_t next = BSON::parseString(buffer, length, 0, str);
    EXPECT_EQ(36, next);
}
