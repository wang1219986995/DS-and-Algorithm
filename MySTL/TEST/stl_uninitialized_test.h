//
#include <iostream>
#include <cxxabi.h>
#include <stddef.h>
#include <vector>
#include <list>
#include <deque>
#include <algorithm>
#include "gtest/gtest.h"
#include "src/stl_uninitialized.h"
#include "src/stl_construct.h"

#define TYPE_SIMPLIFIED(type) \
    abi::__cxa_demangle(typeid(type).name(), NULL, NULL, NULL)


class Base {
public:
    int value;
    Base() : value(0) {}
    Base(int val) : value(val) {}
    Base& operator=(const Base& b) { value = b.value; return *this; }
    bool operator==(const Base& b) { return value == b.value; }
};

TEST(uninitialized_copy, All)
{
    std::vector<std::string> v = {"123", "abc", "sdfs"};
    std::vector<std::string> test(3);
    MySTL::uninitialized_copy(v.begin(), v.end(), test.begin());
    EXPECT_TRUE(std::equal(v.begin(), v.end(), test.begin()));

    std::vector<Base> vb(10, Base(1));
    Base* pbs = (Base*) malloc(10 * sizeof(Base));
    MySTL::uninitialized_copy(vb.begin(), vb.end(), pbs);
    EXPECT_TRUE(std::equal(vb.begin(), vb.end(), pbs));
}

TEST(uninitialized_copy_n, All)
{
    Base* pb = (Base*) malloc(10 * sizeof(Base));
    Base* test = (Base*) malloc(10 * sizeof(Base));
    for(int i = 0; i < 10; ++i)
        MySTL::construct(&pb[i], Base(i));

    MySTL::uninitialized_copy_n(pb, 10, test);
    EXPECT_TRUE(std::equal(pb, pb+10, test));
}


TEST(uninitialized_fill, All)
{
    Base* pb = (Base*) malloc(10 * sizeof(Base));
    Base* test = (Base*) malloc(10 * sizeof(Base));

    std::uninitialized_fill(pb, pb + 10, Base(10));
    MySTL::uninitialized_fill(test, test + 10, Base(10));
    EXPECT_TRUE(std::equal(pb, pb+10, test));
}

TEST(uninitialized_fill_n, All)
{
    Base* pb = (Base*) malloc(10 * sizeof(Base));
    Base* test = (Base*) malloc(10 * sizeof(Base));

    std::uninitialized_fill_n(pb, 10, Base(10));
    MySTL::uninitialized_fill_n(test, 10, Base(10));
    EXPECT_TRUE(std::equal(pb, pb+10, test));
}


//int main(int argc, char **argv)
//{
//
//    ::testing::InitGoogleTest(&argc, argv);
//    RUN_ALL_TESTS();
//
//    std::cout << "Press enter to exit..." << std::endl;
//    std::cin.get();
//
//    return 0;
//}
