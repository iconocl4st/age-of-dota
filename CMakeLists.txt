cmake_minimum_required(VERSION 3.15)
project(age-of-dota)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

set(Boost_USE_STATIC_LIBS ON)

find_package(Boost REQUIRED COMPONENTS system log)
find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
find_package(GLEW REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package (Threads REQUIRED)



include(${CMAKE_CURRENT_SOURCE_DIR}/common.cmake)
include(${CMAKE_CURRENT_SOURCE_DIR}/common_client.cmake)
include(${CMAKE_CURRENT_SOURCE_DIR}/client_ui.cmake)
include(${CMAKE_CURRENT_SOURCE_DIR}/client_script.cmake)
include(${CMAKE_CURRENT_SOURCE_DIR}/server.cmake)
include(${CMAKE_CURRENT_SOURCE_DIR}/ui_lib.cmake)


add_executable(
        both

        src/both/app/run_both.cpp

        src/both/app/run_spectator_client.cpp
        src/both/app/run_spectator_client.h
        src/both/app/test_stuff.cpp

        src/common/state/quad_tree2/test_quadtree.cpp
        src/common/state/quad_tree2/test_quadtree.h
)

target_include_directories(
        both PUBLIC

        ${OPENGL_INCLUDE_DIRS}

        ${QT_INCLUDES}
        ${Qt5Widgets_INCLUDE_DIRS}
        ${Qt5Widgets_LIBRARIES}
        ${Qt5Declarative_INCLUDE_DIRS}

        ${CMAKE_CURRENT_SOURCE_DIR}/ui/src/
)

target_link_libraries(
        both

        server_lib
        client_ui_lib
        client_script_lib
        common_client_lib
        common_lib

        qt_ui_library


        ${OPENGL_LIBRARIES}
        ${GLUT_LIBRARY}
        ${GLEW_LIBRARIES}
        ${Boost_LIBRARIES}
        Qt5::Widgets

        ${CMAKE_DL_LIBS}
        ${Backtrace_LIBRARY}

        pthread
        backtrace
)
