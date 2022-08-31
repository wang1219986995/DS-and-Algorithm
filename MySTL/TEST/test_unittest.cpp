//
// Created by GW00243464 on 2022/8/30.
//

#include "gtest/gtest.h"

int add(int a, int b)
{
    return a + b;
}


TEST(testCase, test0)
{
    EXPECT_EQ(add(20, 10), 30);
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);


    // return RUN_ALL_TESTS();
}
