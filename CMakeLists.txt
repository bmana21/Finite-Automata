cmake_minimum_required(VERSION 3.25)
project(Theory)

set(CMAKE_CXX_STANDARD 17)

include_directories(include)

add_executable(Theory build.cpp NFA.cpp state.cpp)