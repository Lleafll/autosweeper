cmake_minimum_required(VERSION 3.21)

project(aswtest)

add_executable(${PROJECT_NAME}
        MineFieldTest.cpp)
target_include_directories(${PROJECT_NAME} PRIVATE ../include/asw ../src)
find_package(GTest CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE GTest::gtest_main asw)

include(GoogleTest)
gtest_discover_tests(${PROJECT_NAME})