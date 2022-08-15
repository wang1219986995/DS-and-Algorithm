#include <iostream>
// #include "src/alloc.h"

void test() noexcept {};

int main() {
    std::cout << "Hello, World!" << std::endl;
    std::cout << "size_t max is :" << size_t(-1) << std::endl;

    return 0;
}
