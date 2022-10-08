cmake_minimum_required(VERSION 3.21)

project(asw)

add_library(${PROJECT_NAME} STATIC
        src/PlayingField.cpp
        src/predict_mines.cpp)
target_include_directories(${PROJECT_NAME}
        PUBLIC include
        PRIVATE include/asw src)
find_package(Microsoft.GSL CONFIG REQUIRED)
find_package(mdspan CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PUBLIC Microsoft.GSL::GSL std::mdspan)

add_subdirectory(test)