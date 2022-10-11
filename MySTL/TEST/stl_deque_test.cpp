//
#include <iostream>
#include <cxxabi.h>
#include <stddef.h>
#include <vector>
#include <list>
#include <deque>
#include <type_traits>

#include "src/stl_deque.h"
#include "src/stl_vector.h"
#include "src/type_traits.h"
#include "gtest/gtest.h"

#define TYPE_SIMPLIFIED(type) \
    abi::__cxa_demangle(typeid(type).name(), NULL, NULL, NULL)
namespace MySTL
{

class Base
{
public:
    Base() {}
private:

};


TEST(deque_iterator, All)
{
    Base** node_map = (Base**) malloc(10 * sizeof(Base*));
    Base** cur = node_map;
    for(int i = 0; i < 10; ++i)
    {
        Base* tmp = (Base*) malloc(512);
        *cur = tmp;
    }
    MySTL::Deque_iterator<Base, Base&, Base*> first;
    MySTL::Deque_iterator<Base, Base&, Base*> last;
    first.M_set_node(node_map);
    first.M_cur = first.M_first;
    last.M_set_node(node_map + 9);
    last.M_cur = last.M_last;

    MySTL::Deque_iterator<Base, Base&, Base*> tmp;
    tmp.M_set_node(node_map + 9);
    tmp.M_cur = tmp.M_first;
    last = last - last.S_buffer_size();
    EXPECT_TRUE(last == tmp);

    tmp.M_set_node(node_map);
    tmp.M_cur = tmp.M_first;
    tmp += tmp.S_buffer_size();
    first.M_set_node(node_map + 1);
    first.M_cur = first.M_first;
    EXPECT_TRUE(first == tmp);
}




TEST(deque, construct)
{
    std::deque<std::string> d;
    MySTL::deque<std::string> test;
    EXPECT_TRUE(std::equal(d.begin(), d.end(), test.begin()));

    std::deque<std::string> d1(300, "123");
    MySTL::deque<std::string> test1(300, "123");
    EXPECT_TRUE(std::equal(d1.begin(), d1.end(), test1.begin()));
    d1.clear();
    test1.clear();

    std::deque<std::string> d2(300);
    MySTL::deque<std::string> test2(300);
    EXPECT_TRUE(std::equal(d2.begin(), d2.end(), test2.begin()));
    d2.clear();
    test2.clear();

    std::vector<std::string> vs;
    MySTL::vector<std::string> vm;
    for(int i = 0; i < 1000; i++)
    {
        std::string tmp(5, '0');
        for(int i = 0; i < tmp.size(); ++i)
            tmp[i] = '0' + std::rand() % 10;
        vs.push_back(tmp);
        vm.push_back(tmp);
    }
    std::deque<std::string> d3(vs.begin(), vs.end());
    MySTL::deque<std::string> test3(vm.begin(), vm.end());
    EXPECT_TRUE(std::equal(d3.begin(), d3.end(), test3.begin()));

    std::deque<std::string> d4(d3);
    MySTL::deque<std::string> test4(test3);
    EXPECT_TRUE(std::equal(d4.begin(), d4.end(), test4.begin()));

    std::deque<std::string> d5 = d4;
    MySTL::deque<std::string> test5 = test4;
    EXPECT_TRUE(std::equal(d5.begin(), d5.end(), test5.begin()));
}

TEST(deque, insert)
{
    std::deque<std::string> d1;
    std::deque<std::string> test1;

    d1.insert(d1.begin(), "123");
    test1.insert(test1.begin(), "123");

    d1.insert(d1.begin(), 100, "abc");
    test1.insert(test1.begin(), 100, "abc");

    std::vector<std::string> v;
    for(int i = 0; i < 100; ++i)
    {
        std::string tmp(5, '0');
        for(int i = 0; i < tmp.size(); ++i)
            tmp[i] = '0' + std::rand() % 10;
        v.push_back(tmp);
        int pos = std::rand() % d1.size();
        d1.insert(d1.begin() + pos, tmp);
        test1.insert(test1.begin() + pos, tmp);
    }

    int pos = std::rand() % d1.size();
    d1.insert(d1.begin() + pos, v.begin(), v.end());
    test1.insert(test1.begin() + pos, v.begin(), v.end());
    EXPECT_TRUE(std::equal(d1.begin(), d1.end(), test1.begin()));
}

TEST(deque, push_and_pop)
{
    std::deque<std::string> d;
    MySTL::deque<std::string> test;

    for(int i = 0; i < 1000; ++i)
    {
        std::string tmp(10, '0');
        for(int i = 0; i < tmp.size(); ++i)
            tmp[i] = '0' + std::rand() % 10;
        d.push_back(tmp);
        d.push_front(tmp);
        test.push_back(tmp);
        test.push_front(tmp);
    }
    EXPECT_TRUE(std::equal(test.begin(), test.end(), d.begin()));

    for(int i = 0; i < 400; ++i)
    {
        d.pop_back();
        test.pop_back();
        d.pop_front();
        test.pop_front();
    }
    EXPECT_TRUE(std::equal(test.begin(), test.end(), d.begin()));
}

TEST(deque, other)
{
    std::deque<std::string> d;
    MySTL::deque<std::string> test;
    EXPECT_EQ(d.empty(), test.empty());

    for(int i = 0; i < 1000; ++i)
    {
        std::string tmp(10, '0');
        for(int i = 0; i < tmp.size(); ++i)
            tmp[i] = '0' + std::rand() % 10;
        d.push_back(tmp);
        d.push_front(tmp);
        test.push_back(tmp);
        test.push_front(tmp);
    }
    EXPECT_TRUE(std::equal(test.begin(), test.end(), d.begin()));
    EXPECT_EQ(d.size(), test.size());

    int n = std::rand() % (d.size() / 2);
    d.erase(d.begin() + n, d.begin() + n + (d.size()/4));
    test.erase(test.begin() + n, test.begin() + n + (test.size()/4));
    EXPECT_EQ(d.size(), test.size());
    EXPECT_TRUE(std::equal(test.begin(), test.end(), d.begin()));

    d.resize(d.size() * 2);
    test.resize(test.size() * 2);
    EXPECT_EQ(d.size(), test.size());
    EXPECT_TRUE(std::equal(test.begin(), test.end(), d.begin()));

    d.resize(d.size() / 4);
    test.resize(test.size() / 4);
    EXPECT_EQ(d.size(), test.size());
    EXPECT_TRUE(std::equal(test.begin(), test.end(), d.begin()));

    d.clear();
    test.clear();
    EXPECT_EQ(d.size(), test.size());
}


}



int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    RUN_ALL_TESTS();

    std::cout << "Press enter to exit..." << std::endl;
    std::cin.get();
    return 0;
}