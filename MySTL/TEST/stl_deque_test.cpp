//
// Created by GW00243464 on 2022/9/29.
//
#include <iostream>
#include <cxxabi.h>
#include <stddef.h>
#include <vector>
#include <list>
#include <deque>
#include "gtest/gtest.h"
#include "src/stl_deque.h"

#define TYPE_SIMPLIFIED(type) \
    abi::__cxa_demangle(typeid(type).name(), NULL, NULL, NULL)

class Base
{
public:
    Base() {}
private:

};


TEST(deque_iterator, All)
{
    std::cout << "123" << std::endl;
    Base** node_map = (Base**) malloc(10 * sizeof(Base*));
    Base** cur = node_map;
    std::cout << "123" << std::endl;
    for(int i = 0; i < 10; ++i)
    {
        Base* tmp = (Base*) malloc(512);
        *cur = tmp;
    }
    std::cout << "123" << std::endl;
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

    tmp.M_set_node(node_map + 1);
    tmp.M_cur = tmp.M_first;
    EXPECT_TRUE(tmp == (first + first.S_buffer_size() + 1));
    tmp.M_cur == tmp.M_last;
    EXPECT_TRUE(tmp == (first + 2 * first.S_buffer_size()));

}



int main(int argc, char **argv)
{
    std::cout << "123" << std::endl;
    ::testing::InitGoogleTest(&argc, argv);
    RUN_ALL_TESTS();

    std::cout << "Press enter to exit..." << std::endl;
    std::cin.get();
    return 0;
}