cmake_minimum_required(VERSION 3.28)
project(basic_cpp_interpreter)

set(CMAKE_CXX_STANDARD 17)

add_executable(basic_cpp_interpreter main.cpp
        Parser.cpp
        Parser.h
        Lexer.cpp
        Lexer.h
        AbstractSyntaxTree.cpp
        AbstractSyntaxTree.h)
