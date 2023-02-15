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
#include "gtest/gtest.h"
#include "stdio.h"
#include "src/stl_alloc.h"
#include "src/stl_btree.h"
#include "src/stl_map.h"


int main( )
{
    MySTL::map<int, int> m;
    for(int i = 0; i < 1000; ++i)
        m.insert(MySTL::pair<int, int>(i, i+1));

    for(MySTL::map<int, int>::iterator first = m.begin(); first != m.end(); ++first)
        (*first).second = 1;


    std::cout << "finish" << std::endl;

}




