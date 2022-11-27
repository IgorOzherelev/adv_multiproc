cmake_minimum_required(VERSION 3.23)

project(adv_multiproc)
add_subdirectory(quicksort)

set(CMAKE_CXX_STANDARD 14)

add_executable(adv_multiproc main.cpp)