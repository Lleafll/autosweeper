cmake_minimum_required(VERSION 3.21)

project(aswtest)

add_executable(${PROJECT_NAME}
        Algorithm2DTest.cpp
        CellTest.cpp
        PlayingFieldTest.cpp
        PredictMinesTest.cpp)
target_include_directories(${PROJECT_NAME} PRIVATE ../include/asw ../src)
target_compile_options(${PROJECT_NAME} PRIVATE
    $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:GNU>>:-Wall -Wpedantic -Wextra -Werror>
    $<$<CXX_COMPILER_ID:MSVC>:/W4 /WX /permissive- /EHsc /utf-8>)

find_package(Catch2 CONFIG REQUIRED)
find_package(fmt CONFIG REQUIRED)
find_package(magic_enum CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE Catch2::Catch2WithMain fmt::fmt magic_enum::magic_enum asw)

include(CTest)
include(Catch)
catch_discover_tests(${PROJECT_NAME})