cmake_minimum_required(VERSION 3.21)

project(autosweeper)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_EXTENSIONS OFF)
if (CMAKE_CXX_COMPILER_FRONTEND_VARIANT STREQUAL "MSVC")
    add_compile_options(/EHsc /W4 /WX /permissive- /utf-8)
elseif (CMAKE_CXX_COMPILER_FRONTEND_VARIANT STREQUAL "GNU")
    add_compile_options(-Wall -Wextra -Wshadow -Wconversion -Wpedantic -Werror)
endif ()
enable_testing()

add_subdirectory(di_ptr)
add_subdirectory(asw)
add_subdirectory(aswui)