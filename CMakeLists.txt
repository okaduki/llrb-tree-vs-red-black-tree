cmake_minimum_required(VERSION 3.4)
project(mergesort CXX)
add_executable(a.out main.cpp)

set(CMAKE_CXX_STANDARD, 14)
set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-O3 -g -std=c++14 -Wall -fopenmp")
