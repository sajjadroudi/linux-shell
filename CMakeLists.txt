cmake_minimum_required(VERSION 3.20)
project(myshell)

set(CMAKE_CXX_STANDARD 20)

add_executable(myshell main.cpp CommandExecutor.hpp utils.hpp Command.hpp Shell.hpp AliasHandler.hpp)
