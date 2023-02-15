//
#include <iostream>
#include <cxxabi.h>
#include <stddef.h>
#include <vector>
#include <list>
#include <deque>
#include "gtest/gtest.h"
#include "src/stl_vector.h"
using namespace std;

#define TYPE_SIMPLIFIED(type) \
    abi::__cxa_demangle(typeid(type).name(), NULL, NULL, NULL)

TEST(Vector_alloc_base, All)
{
    MySTL::Vector_alloc_base<int, MySTL::allocator<int>, false> alloc_base;
    MySTL::allocator<int> allocator = alloc_base.get_allocator();

    EXPECT_STREQ(TYPE_SIMPLIFIED(allocator), "MySTL::allocator<int>");
    cout << "allocate test " << endl;
    int* p = allocator.allocate(1);
    EXPECT_NE(*p, 0);
}



template<typename T>
std::ostream& operator<<(std::ostream& s, const std::vector<T>& v)
{
    s.put('[');
    char comma[3] = {'\0', ' ', '\0'};
    for (const auto& e : v)
    {
        s << comma << e;
        comma[0] = ',';
    }
    return s << ']';
}

TEST(vector, construct)
{
    std::vector<int> v = {1,2,3,4,5};
    MySTL::vector<int> test(&v[0], &v[v.size()]);
    EXPECT_TRUE(std::equal(v.begin(), v.end(), test.begin()));

    MySTL::vector<int> test2(10, 10);
    std::vector<int> v2(10,10);
    EXPECT_TRUE(std::equal(v2.begin(), v2.end(), test2.begin()));

    MySTL::vector<int> test3(10);
    std::vector<int> v3(10);
    EXPECT_TRUE(std::equal(v3.begin(), v3.end(), test3.begin()));


    MySTL::vector<int> test4(test);
    std::vector<int> v4(v);
    EXPECT_TRUE(std::equal(v4.begin(), v4.end(), test4.begin()));

}



TEST(vector, push_and_insert)
{
    std::vector<std::string> v;
    MySTL::vector<std::string> test;

    for(int i = 0; i < 100; ++i)
    {
        int rand = std::rand() % 10;
        std::string temp(rand, 0);
        for(int i = 0; i < temp.size(); ++i)
            temp[i] += std::rand() % 128;
        v.push_back(temp);
        test.push_back(temp);
    }
    EXPECT_TRUE(std::equal(v.begin(), v.end(), test.begin()));

    std::vector<std::string> v_copy(v);
    MySTL::vector<std::string> test_copy(test);
    EXPECT_TRUE(std::equal(v_copy.begin(), v_copy.end(), test_copy.begin()));
    for(int i = 0; i < 30; ++i)
    {
        int pos = std::rand() % test_copy.size();
        v.insert(v.begin() + pos, v_copy[pos]);
        test.insert(test.begin() + pos, test_copy[pos]);

        if( i % 10 == 0)
        {
            v.insert(v.begin() + pos, v_copy.begin(), v_copy.begin() + 5);
            test.insert(test.begin() + pos, test_copy.begin(), test_copy.begin() + 5);
        }
    }
    EXPECT_TRUE(std::equal(v.begin(), v.end(), test.begin()));
}


TEST(vector, other)
{
    MySTL::vector<std::string> test;
    std::vector<std::string> v;

    EXPECT_EQ(test.empty(), v.empty());
    EXPECT_EQ(test.size(), v.size());
    EXPECT_EQ(test.capacity(), v.capacity());
    for(int i = 0; i < 200; ++i)
    {
        int rand = std::rand() % 10;
        std::string temp(rand, 0);
        for(int i = 0; i < temp.size(); ++i)
            temp[i] += std::rand() % 128;
        v.push_back(temp);
        test.push_back(temp);
    }
    EXPECT_EQ(test.empty(), v.empty());
    EXPECT_EQ(test.size(), v.size());
    EXPECT_EQ(test.capacity(), v.capacity());

    int cap = test.capacity();
    v.reserve(2 * cap);
    test.reserve(2 * cap);
    EXPECT_EQ(test.capacity(), v.capacity());

    v.resize(v.size() * 2, " ");
    test.resize(test.size() * 2, " ");
    EXPECT_TRUE(std::equal(v.begin(), v.end(), test.begin()));


    v.resize(v.size() / 3, " ");
    test.resize(test.size() / 3, " ");
    EXPECT_TRUE(std::equal(v.begin(), v.end(), test.begin()));

    for(int i = 0; i < test.size()/2; ++i)
    {
        v.pop_back();
        test.pop_back();
    }
    EXPECT_TRUE(std::equal(v.begin(), v.end(), test.begin()));

    v.erase(v.begin(), v.begin() + v.size()/2);
    test.erase(test.begin(), test.begin() + test.size()/2);
    EXPECT_TRUE(std::equal(v.begin(), v.end(), test.begin()));
    v.clear();
    test.clear();
    EXPECT_EQ(test.empty(), v.empty());
    EXPECT_EQ(test.size(), v.size());
    EXPECT_EQ(test.capacity(), v.capacity());
}



//int main(int argc, char **argv)
//{
//
//    ::testing::InitGoogleTest(&argc, argv);
//    RUN_ALL_TESTS();
//
//    MySTL::vector<std::string> test;
//    test.push_back("the");
//
//    cout << "Press enter to exit..." << endl;
//    cin.get();
//
//    return 0;
//}


