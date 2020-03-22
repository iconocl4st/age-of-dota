
include_directories(${PROJECT_SOURCE_DIR})
file(GLOB QT_LIB_SRC_FILES
        ${PROJECT_SOURCE_DIR}/ui/build/*.cpp
        ${PROJECT_SOURCE_DIR}/ui/build/*.h
        ${PROJECT_SOURCE_DIR}/ui/src/*.cpp
        ${PROJECT_SOURCE_DIR}/ui/src/*.h
)

add_library(qt_ui_library STATIC

        ${QT_LIB_SRC_FILES}
)


target_include_directories(
    qt_ui_library PUBLIC

    ${CMAKE_CURRENT_SOURCE_DIR}/ui/src/
    ${CMAKE_CURRENT_SOURCE_DIR}/ui/build/

    ${QT_INCLUDES}
    ${Qt5Widgets_INCLUDE_DIRS}
    ${Qt5Widgets_LIBRARIES}
    ${Qt5Declarative_INCLUDE_DIRS}
)