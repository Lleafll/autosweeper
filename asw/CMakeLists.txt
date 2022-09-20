cmake_minimum_required(VERSION 3.21)

project(asw)

add_library(${PROJECT_NAME} STATIC
        src/MineField.cpp
        src/ObviousMineFinder.cpp
        src/PlayingField.cpp
        src/Solver.cpp)
target_include_directories(${PROJECT_NAME}
        PUBLIC include
        PRIVATE include/asw src)
find_package(mdspan CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE std::mdspan)

add_subdirectory(test)