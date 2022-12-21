cmake_minimum_required(VERSION 3.21)

project(asw)

add_library(${PROJECT_NAME} STATIC
    src/Cell.cpp
    src/find_in_image.cpp
    src/InMemoryPlayingField.cpp
    src/MinesweeperScreen.cpp
    src/predict_mines.cpp
    src/ImageMatchingPlayingField.cpp)
target_include_directories(${PROJECT_NAME}
    PUBLIC include
    PRIVATE include/asw src)
if (CMAKE_CXX_COMPILER_FRONTEND_VARIANT STREQUAL "MSVC")
    target_compile_options(${PROJECT_NAME} PRIVATE /W4 /WX /permissive- /EHsc /utf-8)
elseif (CMAKE_CXX_COMPILER_FRONTEND_VARIANT STREQUAL "GNU")
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wpedantic -Wextra -Werror)
endif ()

find_path(PNGPP_INCLUDE_DIRS "png++/color.hpp")
target_include_directories(${PROJECT_NAME} PRIVATE ${PNGPP_INCLUDE_DIRS})
find_package(Microsoft.GSL CONFIG REQUIRED)
find_package(mdspan CONFIG REQUIRED)
find_package(PNG REQUIRED)
target_link_libraries(${PROJECT_NAME}
    PUBLIC di_ptr Microsoft.GSL::GSL std::mdspan
    PRIVATE PNG::PNG)

add_subdirectory(test)