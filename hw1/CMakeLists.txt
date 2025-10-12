cmake_minimum_required(VERSION 3.27)
project(hw1)

set(CMAKE_CXX_STANDARD 17)

add_executable(hw1 simulated_test.cpp
        graph_operations.cpp
        graph_operations.h
        graph_simulator.cpp)
