cmake_minimum_required(VERSION 3.5)

project(untitled LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
include_directories(
	Chapter 4/
)
add_executable(untitled main.cpp "Chapter 4/2. MatrixMutlipy .h")
