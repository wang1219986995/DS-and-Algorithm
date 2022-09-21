//
// Created by GW00243464 on 2022/9/20.
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

TEST(Vector_base, All)
{
//    MySTL::Vector_base<int, MySTL::allocator<int>> v(MySTL::allocator<int>());
//    cout << TYPE_SIMPLIFIED(v) << endl;
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

TEST(vector, All)
{
    std::vector<std::string> words{"the", "frogurt", "is", "also", "cursed"};


    std::vector<std::string> words1 {"the", "frogurt", "is", "also", "cursed"};
    std::cout << "words1: " << words1 << '\n';

    // words2 == words1
    std::vector<std::string> words2(words1.begin(), words1.end());
    std::cout << "words2: " << words2 << '\n';

    // words3 == words1
    std::vector<std::string> words3(words1);
    std::cout << "words3: " << words3 << '\n';

    // words4 is {"Mo", "Mo", "Mo", "Mo", "Mo"}
    std::vector<std::string> words4(5, "Mo");
    std::cout << "words4: " << words4 << '\n';
}




#include <vector>
int main(int argc, char **argv)
{

    ::testing::InitGoogleTest(&argc, argv);
    RUN_ALL_TESTS();


    MySTL::vector<std::string> test;
    test.push_back("the");

    cout << "Press enter to exit..." << endl;
    cin.get();

    return 0;
}


