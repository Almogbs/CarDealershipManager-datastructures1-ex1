cmake_minimum_required(VERSION 3.0.0)
project(avltree)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_C++_FLAGS} -Wall -pedantic-errors -Werror -DNDEBUG")

add_executable(avltree_test main.cpp avl_tree.h node.h exceptions.h )