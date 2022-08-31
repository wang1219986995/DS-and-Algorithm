cmake_minimum_required(VERSION 3.22)
project(MySTL)

set(CMAKE_CXX_STANDARD 20)

include_directories(
    ${PROJECT_SOURCE_DIR}
    ${PROJECT_SOURCE_DIR}/TEST/include
)

link_directories(
        ${PROJECT_SOURCE_DIR}/TEST/lib
)

add_executable(vector_and_list vector_and_list_test.cpp)
add_executable(stack_and_queue stack_and_queue.cpp)
add_executable(my_heap my_heap.cpp)
#add_executable(test TEST/test_unittest.cpp)
#
#target_link_libraries(test
#        gtest
#        gtest_main
#)