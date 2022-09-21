cmake_minimum_required(VERSION 3.22)
project(MySTL)

set(CMAKE_CXX_STANDARD 20)

include_directories(
    ${PROJECT_SOURCE_DIR}
    ${PROJECT_SOURCE_DIR}/TEST/include
)

link_directories(
        ${PROJECT_SOURCE_DIR}/TEST/lib/
)


add_executable(main main.cpp)
add_executable(sort sort_algorithm.cpp)





add_executable(test_stl_iterator TEST/stl_iterator_test.cpp)
target_link_libraries(test_stl_iterator gtest)

add_executable(test_stl_alloc TEST/stl_alloc_test.cpp)
target_link_libraries(test_stl_alloc gtest)

add_executable(test_stl_vector TEST/stl_vector_test.cpp)
target_link_libraries(test_stl_vector gtest)
