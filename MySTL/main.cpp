#include <iostream>
#include <pair>
// #include "src/alloc.h"


int main() {
    std::cout << "Hello, World!" << std::endl;
    std::cout << "size_t max is :" << size_t(-1) << std::endl;
    std::pair<int, int> a;
    std::pair<int, int> b = std::pair(a);


    bool res = (a == b);
    return 0;
}
