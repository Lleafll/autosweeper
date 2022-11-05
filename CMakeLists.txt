cmake_minimum_required(VERSION 3.21)

project(asw)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_EXTENSIONS OFF)
enable_testing()

add_subdirectory(di_ptr)
add_subdirectory(asw)
add_subdirectory(aswui)