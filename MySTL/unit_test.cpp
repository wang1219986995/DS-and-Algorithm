#include "gtest/gtest.h"
#include "src/stl_algo.h"
#include "src/stl_alloc.h"
#include "src/stl_iterator.h"
#include "src/stl_pair.h"
#include "src/stl_vector.h"
#include "TEST/stl_vector_test.h"
#include "TEST/stl_uninitialized_test.h"
#include "TEST/stl_iterator_test.h"
#include "TEST/stl_alloc_test.h"




int main (int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    RUN_ALL_TESTS();

    MySTL::vector<std::string> test;
    test.push_back("the");

    std::cout << "Press enter to exit..." << std::endl;
    std::cin.get();

    return 0;
    return 0;
}
