cmake_minimum_required(VERSION 3.21)

project(asw)

add_library(${PROJECT_NAME} STATIC
    src/Cell.cpp
    src/find_in_image.cpp
    src/ImageMatchingPlayingField.cpp
    src/InMemoryPlayingField.cpp
    src/MinesweeperScreen.cpp
    src/predict_mines.cpp)
target_include_directories(${PROJECT_NAME}
    PUBLIC include
    PRIVATE include/asw src)

find_path(PNGPP_INCLUDE_DIRS "png++/color.hpp")
target_include_directories(${PROJECT_NAME} PRIVATE ${PNGPP_INCLUDE_DIRS})
find_package(Microsoft.GSL CONFIG REQUIRED)
find_package(mdspan CONFIG REQUIRED)
find_package(PNG REQUIRED)
find_package(proxy CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME}
    PUBLIC Microsoft.GSL::GSL msft_proxy std::mdspan
    PRIVATE PNG::PNG)

add_subdirectory(test)