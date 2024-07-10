cmake_minimum_required(VERSION 3.28)
project(basic_cpp_interpreter)

set(CMAKE_CXX_STANDARD 17)

add_executable(basic_cpp_interpreter main.cpp)
add_executable(basic_cpp_interpreter main.cpp
        Lexer.cpp
        Lexer.h
