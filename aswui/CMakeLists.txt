cmake_minimum_required(VERSION 3.21)


project(aswui)

find_package(Qt6 CONFIG REQUIRED COMPONENTS Widgets)
qt_standard_project_setup()

add_library(${PROJECT_NAME} STATIC
    src/CellsWidgetQt.cpp
    src/ConstCellSpanPresenter.cpp
    src/ConstCellSpanWidgetQt.cpp
    src/LoggerWidgetQt.cpp
    src/MinePredictionsPresenter.cpp
    src/MinePredictionsWidgetQt.cpp)
set_property(TARGET ${PROJECT_NAME} PROPERTY AUTOMOC ON)
target_sources(${PROJECT_NAME} PRIVATE  # MOC headers
    include/aswui/CellConstSpanWidgetQt.h
    include/aswui/MinePredictionsWidgetQt.h
    include/aswui/LoggerWidgetQt.h
    src/CellsWidgetQt.h)
target_include_directories(${PROJECT_NAME}
    PUBLIC include
    PRIVATE include/aswui src)
if (CMAKE_CXX_COMPILER_FRONTEND_VARIANT STREQUAL "MSVC")
    target_compile_options(${PROJECT_NAME} PRIVATE /W4 /WX /permissive- /EHsc /utf-8)
elseif (CMAKE_CXX_COMPILER_FRONTEND_VARIANT STREQUAL "GNU")
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wpedantic -Wextra -Werror)
endif ()

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
get_target_property(ASW_SOURCE_DIR asw SOURCE_DIR)
install(DIRECTORY ${ASW_SOURCE_DIR}/resources/minesweeperclassic
    DESTINATION ${CMAKE_INSTALL_BINDIR})
if (WIN32)
    qt_generate_deploy_app_script(
        TARGET ${PROJECT_NAME}
        FILENAME_VARIABLE deploy_script
        NO_UNSUPPORTED_PLATFORM_ERROR)
    install(SCRIPT ${deploy_script})
    install(CODE [[
          file(GET_RUNTIME_DEPENDENCIES
            EXECUTABLES $<TARGET_FILE:aswuimain>
            RESOLVED_DEPENDENCIES_VAR _r_deps
            UNRESOLVED_DEPENDENCIES_VAR _u_deps
            POST_EXCLUDE_REGEXES "system")
          foreach(_file ${_r_deps})
            file(INSTALL
              DESTINATION "${CMAKE_INSTALL_PREFIX}/bin"
              TYPE SHARED_LIBRARY
              FILES "${_file}")
          endforeach()
          list(LENGTH _u_deps _u_length)
          if("${_u_length}" GREATER 0)
            message(WARNING "Unresolved dependencies detected!")
          endif()
        ]])
endif (WIN32)