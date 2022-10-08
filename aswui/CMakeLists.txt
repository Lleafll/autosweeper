cmake_minimum_required(VERSION 3.21)


project(aswui)

find_package(Qt6 CONFIG REQUIRED COMPONENTS Widgets)
qt_standard_project_setup()

add_library(${PROJECT_NAME} STATIC
        src/PlayingFieldPresenter.cpp
        src/PlayingFieldWidgetQt.cpp)
set_property(TARGET ${PROJECT_NAME} PROPERTY AUTOMOC ON)
target_sources(${PROJECT_NAME} PRIVATE  # MOC headers
        include/aswui/PlayingFieldWidgetQt.h)
target_include_directories(${PROJECT_NAME}
        PUBLIC include
        PRIVATE include/aswui src)
find_package(Microsoft.GSL CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PUBLIC Microsoft.GSL::GSL Qt6::Widgets asw)


add_subdirectory(test)


project(aswuimain)

add_executable(${PROJECT_NAME}
        main.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE aswui)