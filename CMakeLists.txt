cmake_minimum_required(VERSION 3.29)
project(p3)

set(CMAKE_CXX_STANDARD 20)

add_executable(p3 main.cpp
        Node.h
        Movie.h
        MaxHeap.h
        BPlusTree.cpp)
