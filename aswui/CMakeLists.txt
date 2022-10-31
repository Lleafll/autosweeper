cmake_minimum_required(VERSION 3.21)


project(aswui)

find_package(Qt6 CONFIG REQUIRED COMPONENTS Widgets)
qt_standard_project_setup()

add_library(${PROJECT_NAME} STATIC
        src/CellsWidgetQt.cpp
        src/ConstCellSpanPresenter.cpp
        src/ConstCellSpanWidgetQt.cpp
        src/MinePredictionsPresenter.cpp
        src/MinePredictionsWidgetQt.cpp)
set_property(TARGET ${PROJECT_NAME} PROPERTY AUTOMOC ON)
target_sources(${PROJECT_NAME} PRIVATE  # MOC headers
        include/aswui/ConstCellSpanWidgetQt.h
        include/aswui/MinePredictionsWidgetQt.h
        src/CellsWidgetQt.h)
target_include_directories(${PROJECT_NAME}
        PUBLIC include
        PRIVATE include/aswui src)
if (CMAKE_CXX_COMPILER_FRONTEND_VARIANT STREQUAL "MSVC")
    target_compile_options(${PROJECT_NAME} PRIVATE /W4 /WX /permissive- /EHsc /utf-8)
elseif (CMAKE_CXX_COMPILER_FRONTEND_VARIANT STREQUAL "GNU")
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wpedantic -Wextra -Werror)
endif()

find_package(Microsoft.GSL CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PUBLIC Microsoft.GSL::GSL Qt6::Widgets asw)

add_subdirectory(test)


project(aswuimain)

qt_standard_project_setup()
qt_add_executable(${PROJECT_NAME}
        main.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE aswui)

install(TARGETS ${PROJECT_NAME}
    BUNDLE DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
if (WIN32)
    qt_generate_deploy_app_script(
        TARGET ${PROJECT_NAME}
        FILENAME_VARIABLE deploy_script
        NO_UNSUPPORTED_PLATFORM_ERROR)
    install(SCRIPT ${deploy_script})
endif (WIN32)