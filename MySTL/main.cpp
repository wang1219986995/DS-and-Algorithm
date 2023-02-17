#include <algorithm>
#include <iostream>
#include <vector>
#include <bitset>
#include <memory>
#include <queue>
#include <deque>
#include <stddef.h>
#include <math.h>
#include <functional>
#include <string.h>
#include <unordered_map>
#include <unordered_set>
#include <string>
#include <deque>
#include "gtest/gtest.h"
#include "stdio.h"
#include "src/stl_alloc.h"
#include "src/stl_btree.h"
#include "src/stl_map.h"


int main( )
{
//    MySTL::avl_map<int, int> m;
//    m.insert(MySTL::pair<int,int>(1,1));

    int a = 0x1234;
    char* b = (char*) &a;
    for(int i = 0; i < 4; ++i)
        printf("%p ", *(b+i));

    std::cout << "finish" << std::endl;

}



