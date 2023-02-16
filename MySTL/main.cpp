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
    MySTL::avl_map<int, int> m;
    m.insert(MySTL::pair<int,int>(1,1));

    std::cout << "finish" << std::endl;

}



