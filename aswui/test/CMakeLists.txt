cmake_minimum_required(VERSION 3.21)

project(aswuitest)

add_executable(${PROJECT_NAME}
        ConstCellSpanPresenterTest.cpp
        MinePredictionsPresenterTest.cpp)
target_include_directories(${PROJECT_NAME} PRIVATE ../include/aswui ../src)
if (CMAKE_CXX_COMPILER_FRONTEND_VARIANT STREQUAL "MSVC")
    target_compile_options(${PROJECT_NAME} PRIVATE /W4 /WX /permissive- /EHsc /utf-8)
elseif (CMAKE_CXX_COMPILER_FRONTEND_VARIANT STREQUAL "GNU")
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wpedantic -Wextra -Werror)
endif()

find_package(Catch2 CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE Catch2::Catch2WithMain aswui)

include(CTest)
include(Catch)
catch_discover_tests(${PROJECT_NAME})
