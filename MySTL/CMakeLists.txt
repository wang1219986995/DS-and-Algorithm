cmake_minimum_required(VERSION 3.1)
project(MySTL)

set(CMAKE_CXX_STANDARD 11)

include_directories(
    ${PROJECT_SOURCE_DIR}
    ${PROJECT_SOURCE_DIR}/TEST/include
)

link_directories(
        ${PROJECT_SOURCE_DIR}/TEST/lib/
)


add_executable(main
        main.cpp
        src/stl_hash_map.h
        src/stl_map.h
        src/stl_set.h
        src/stl_multimap.h
        src/stl_multiset.h
        src/stl_function.h
        src/stl_btree.h)


add_executable(test_stl_iterator TEST/stl_iterator_test.cpp)
target_link_libraries(test_stl_iterator gtest)

add_executable(test_stl_alloc TEST/stl_alloc_test.cpp)
target_link_libraries(test_stl_alloc gtest)

add_executable(test_stl_vector TEST/stl_vector_test.cpp)
target_link_libraries(test_stl_vector gtest)

add_executable(test_stl_uninitialized TEST/stl_uninitialized_test.cpp)
target_link_libraries(test_stl_uninitialized gtest)

add_executable(test_stl_construct TEST/stl_construct_test.cpp)
target_link_libraries(test_stl_construct gtest)

add_executable(test_stl_deque TEST/stl_deque_test.cpp)
target_link_libraries(test_stl_deque gtest)

add_executable(test_stl_algo TEST/stl_algo_test.cpp)
target_link_libraries(test_stl_algo gtest)
