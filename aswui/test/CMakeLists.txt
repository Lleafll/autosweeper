cmake_minimum_required(VERSION 3.21)

project(aswuitest)

add_executable(${PROJECT_NAME}
        ConstCellSpanPresenterTest.cpp
        MinePredictionsPresenterTest.cpp)
target_include_directories(${PROJECT_NAME} PRIVATE ../include/aswui ../src)
target_compile_options(${PROJECT_NAME} PRIVATE
    $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:GNU>>:-Wall -Wpedantic -Wextra -Werror>
    $<$<CXX_COMPILER_ID:MSVC>:/W4 /WX /permissive- /EHsc /utf-8>)

find_package(Catch2 CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE Catch2::Catch2WithMain aswui)

include(CTest)
include(Catch)
catch_discover_tests(${PROJECT_NAME})
