cmake_minimum_required(VERSION 3.21)

project(aswuitest)

add_executable(${PROJECT_NAME}
        PlayingFieldPresenterTest.cpp)
target_include_directories(${PROJECT_NAME} PRIVATE ../include/aswui ../src)

find_package(Catch2 CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE Catch2::Catch2WithMain aswui)

include(CTest)
include(Catch)
catch_discover_tests(${PROJECT_NAME})
