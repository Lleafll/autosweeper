cmake_minimum_required(VERSION 3.21)

project(asw)

add_library(${PROJECT_NAME} STATIC
        src/MineField.cpp
        src/PlayingField.cpp)
target_include_directories(${PROJECT_NAME}
        PUBLIC include
        PRIVATE include/msw src)

add_subdirectory(test)