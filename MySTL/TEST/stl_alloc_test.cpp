//
// Created by GW00243464 on 2022/9/19.
//
#include <iostream>
#include <cxxabi.h>
#include <stddef.h>
#include <vector>
#include <list>
#include <deque>
#include "gtest/gtest.h"
#include "src/stl_alloc.h"
using namespace std;

#define TYPE_SIMPLIFIED(type) \
    abi::__cxa_demangle(typeid(type).name(), NULL, NULL, NULL)

TEST(simple_alloc, All)
{
    // MySTL::simple_alloc<int,
}


TEST(allocator, All)
{
    MySTL::allocator<int> alloc;

    int a = 10;
    EXPECT_EQ(alloc.address(a), &a);
    EXPECT_EQ((long long)(alloc.allocate(0)), NULL);

}


int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);

    RUN_ALL_TESTS();

    cout << "Press enter to exit..." << endl;
    cin.get();
    return 0;
}
