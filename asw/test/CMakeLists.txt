cmake_minimum_required(VERSION 3.21)

project(aswtest)

add_executable(${PROJECT_NAME}
        MineFieldTest.cpp
        PlayingFieldTest.cpp)
target_include_directories(${PROJECT_NAME} PRIVATE ../include/asw ../src)

find_package(Catch2 CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE Catch2::Catch2WithMain asw)

include(CTest)
include(Catch)
catch_discover_tests(${PROJECT_NAME})