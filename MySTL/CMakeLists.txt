cmake_minimum_required(VERSION 3.1)
project(MySTL)

set(CMAKE_CXX_STANDARD 11)

include_directories(
    ${PROJECT_SOURCE_DIR}
    ${PROJECT_SOURCE_DIR}/TEST/include
    ${PROJECT_SOURCE_DIR}/TEST/
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


#add_executable(test_stl_iterator TEST/stl_iterator_test.h)
#target_link_libraries(test_stl_iterator gtest)

#add_executable(test_stl_alloc TEST/stl_alloc_test.h)
#target_link_libraries(test_stl_alloc gtest)
#
#add_executable(test_stl_vector TEST/stl_vector_test.h)
#target_link_libraries(test_stl_vector gtest)

#add_executable(test_stl_uninitialized TEST/stl_uninitialized_test.h)
#target_link_libraries(test_stl_uninitialized gtest)

add_executable(test_stl_construct TEST/stl_construct_test.cpp)
target_link_libraries(test_stl_construct gtest)

add_executable(test_stl_deque TEST/stl_deque_test.cpp)
target_link_libraries(test_stl_deque gtest)

add_executable(test_stl_algo TEST/stl_algo_test.cpp)
target_link_libraries(test_stl_algo gtest)

add_executable(test_stl_map TEST/stl_map_test.cpp)
target_link_libraries(test_stl_map gtest)

file(GLOB UNIT_TEST_SOURCES
        ${PROJECT_SOURCE_DIR}/TEST/*.cpp)

add_executable(unit_test
        unit_test.cpp
        #${UNIT_TEST_SOURCES}
        )

target_link_libraries(unit_test gtest)
