
set(Boost_USE_STATIC_LIBS ON)

add_library(client_ui_lib STATIC
        src/client_ui/app/UiClientContext.cpp
        src/client_ui/app/UiClientContext.h

        src/client_ui/ui/LobbyBrowserAdapter.cpp
        src/client_ui/ui/LobbyBrowserAdapter.h

        src/client_ui/gl/CameraInfo.cpp
        src/client_ui/gl/CameraInfo.h
        src/client_ui/gl/ScreenMap.cpp
        src/client_ui/gl/ScreenMap.h
        src/client_ui/gl/GLConstants.h
        src/client_ui/ui/DragListener.cpp
        src/client_ui/ui/DragListener.h
        src/client_ui/gl/GraphicsModel.cpp
        src/client_ui/gl/GraphicsModel.h
        src/client_ui/gl/Render.cpp
        src/client_ui/gl/Render.h

        src/client_ui/network/ClientMessageHandler.cpp
        src/client_ui/network/ClientMessageHandler.h

        src/client_ui/gl/Color.h

        src/client_ui/ui/GameWindowAdapterImpl.cpp
        src/client_ui/ui/GameWindowAdapterImpl.h

        src/client_ui/state/UiClientGameState.cpp
        src/client_ui/state/UiClientGameState.h
        src/client_ui/gl/DebugRenderingOptions.h
)


target_include_directories(
        client_ui_lib PUBLIC

        ${OPENGL_INCLUDE_DIRS}
        ${GLUT_INCLUDE_DIRS}
        ${GLEW_INCLUDE_DIRS}

        ${QT_INCLUDES}
        ${Qt5Widgets_INCLUDE_DIRS}
        ${Qt5Widgets_LIBRARIES}
        ${Qt5Declarative_INCLUDE_DIRS}

        ${CMAKE_CURRENT_SOURCE_DIR}/ui/src/
)


add_executable(
        client_ui

        src/client_ui/app/ui_client.cpp
)


target_include_directories(
        client_ui PUBLIC

        ${OPENGL_INCLUDE_DIRS}
        ${GLUT_INCLUDE_DIRS}
        ${GLEW_INCLUDE_DIRS}
)


target_link_libraries(
        client_ui

        client_ui_lib
        common_client_lib
        common_lib

        qt_ui_library

        ${OPENGL_LIBRARIES}
        ${GLUT_LIBRARY}
        ${GLEW_LIBRARIES}
        Qt5::Widgets

        ${Boost_LIBRARIES}

        ${CMAKE_DL_LIBS}
        ${Backtrace_LIBRARY}

        pthread
        backtrace
)

