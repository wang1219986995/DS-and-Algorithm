#include <algorithm>
#include <iostream>
#include <vector>
#include <bitset>
#include <memory>
#include <queue>
#include <deque>
#include <stddef.h>
#include <math.h>
#include <string.h>
#include <string>
#include <map>
#include "gtest/gtest.h"
#include <unordered_map>
#include "stdio.h"
#include "btree_set.h"
#include "btree_map.h"
#include <memory>
#include <iostream>
#include <algorithm>
#include <vector>
#include "src/stl_alloc.h"
#include "src/stl_btree.h"
struct Test
{
    int a;
    int b;
    int nums[10];
};

int main( )
{
    MySTL::allocator<int>::rebind<Test>::other alloc;
    auto pp = alloc.allocate(1);
    std::cout << sizeof(*pp) << std::endl;



    std::allocator<int>::rebind<Test>::other std_alloc;
    auto p = std_alloc.allocate(1);
    std::cout << sizeof(*p) << std::endl;

    MySTL::btree_base<int, MySTL::allocator<int>, true> a;
    char* c = a.M_node_allocator.allocate(10);
    std::cout << "----------" << std::endl;
    for(int i = 0; i <= 10; ++i)
        *c = 'a';





}




