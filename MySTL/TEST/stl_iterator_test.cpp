//
// Created by GW00243464 on 2022/8/30.
//
#include <iostream>
#include <cxxabi.h>
#include <stddef.h>
#include <vector>
#include <list>
#include "gtest/gtest.h"
#include "src/stl_iterator.h"
using namespace std;

#define TYPE_SIMPLIFIED(type) \
    abi::__cxa_demangle(typeid(type).name(), NULL, NULL, NULL)

//TODO: 未完待续，后续需整理
// 为什么value_type 要返回 Tp* ?



// stl_iterator_base.h test
TEST(iterator_category, ALL)
{

    EXPECT_STREQ(TYPE_SIMPLIFIED(MySTL::iterator_category(MySTL::input_iterator<int, int64_t>())),
                    "MySTL::input_iterator_tag");

    EXPECT_STREQ(TYPE_SIMPLIFIED(MySTL::iterator_category(MySTL::output_iterator())),
                 "MySTL::output_iterator_tag");

    EXPECT_STREQ(TYPE_SIMPLIFIED(MySTL::iterator_category(MySTL::forward_iterator<int, int64_t>())),
                 "MySTL::forward_iterator_tag");

    EXPECT_STREQ(TYPE_SIMPLIFIED(MySTL::iterator_category(MySTL::bidirectional_iterator<int, int64_t>())),
                 "MySTL::bidirectional_iterator_tag");

    EXPECT_STREQ(TYPE_SIMPLIFIED(MySTL::iterator_category(MySTL::random_access_iterator<int, int64_t>())),
                 "MySTL::random_access_iterator_tag");


    int number = 10;
    const int number2 = 20;
    int* p = &number;
    EXPECT_STREQ(TYPE_SIMPLIFIED(MySTL::iterator_category(p)), "MySTL::random_access_iterator_tag");

    const int *p2 = &number2;
    EXPECT_STREQ(TYPE_SIMPLIFIED(MySTL::iterator_category(p2)), "MySTL::random_access_iterator_tag");
}

TEST(distance_type, All)
{
    //TODO: Tp 类型的distance怎么实例化，怎么测？


    ptrdiff_t a;
    int number = 10;
    const int number2 = 20;
    int* p = &number;
    EXPECT_STREQ(TYPE_SIMPLIFIED(MySTL::distance_type(p)), "long long*");

    const int *p2 = &number2;
    EXPECT_STREQ(TYPE_SIMPLIFIED(MySTL::distance_type(p2)), "long long*");
}

TEST(value_type, All)
{
    struct MyTestType {};
    EXPECT_STREQ(TYPE_SIMPLIFIED(MySTL::value_type(MySTL::random_access_iterator<long long, int64_t>())),
                 "long long*");

    ptrdiff_t a;
    int number = 10;
    const int number2 = 20;
    int* p = &number;
    EXPECT_STREQ(TYPE_SIMPLIFIED(MySTL::value_type(p)), "int*");

    const int *p2 = &number2;
    EXPECT_STREQ(TYPE_SIMPLIFIED(MySTL::value_type(p2)), "int*");

}

// stl_iterator.h test
TEST(back_insert_iterator, All)
{
    vector<int> v;
    vector<int> test;
    MySTL::back_insert_iterator<std::vector<int>> iter(test);
    for(int i = 0; i < 10; ++i)
        v.push_back(i), *iter = i;
    EXPECT_TRUE(std::equal(v.begin(), v.end(), test.begin()));

    test.clear();
    v.clear();
    for(int i = 0; i < 5; ++i)
    {
        MySTL::back_insert(test) = -1;
        v.push_back(-1);
    }
    EXPECT_TRUE(std::equal(v.begin(), v.end(), test.begin()));

    EXPECT_STREQ(TYPE_SIMPLIFIED(MySTL::iterator_category(iter)), "MySTL::output_iterator_tag");
}

TEST(front_insert_iterator, All)
{
    std::list<int> v = {4,5,6};
    std::list<int> test(v);

    MySTL::front_insert_iterator<std::list<int>> iter(test);

    for(int i = 0; i < 10; ++i)
        v.push_front(i), *iter = i;
    EXPECT_TRUE(std::equal(v.begin(), v.end(), test.begin()));

    test.clear();
    v.clear();
    for(int i = 0; i < 5; ++i)
    {
        MySTL::front_inserter(test) = i - 10;
        v.push_front(i - 10);
    }
    EXPECT_TRUE(std::equal(v.begin(), v.end(), test.begin()));

    // EXPECT_STREQ(TYPE_SIMPLIFIED(MySTL::iterator_category(iter)), "MySTL::output_iterator_tag");
}



int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);

    RUN_ALL_TESTS();

    cout << "Press enter to exit..." << endl;
    cin.get();
    return 0;
}
