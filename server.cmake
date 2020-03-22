
set(Boost_USE_STATIC_LIBS ON)

add_library(server_lib STATIC

        src/server/app/ServerSettings.h
        src/server/app/ServerContext.cpp
        src/server/app/ServerContext.h

        src/server/lobby/Lobby.cpp
        src/server/lobby/Lobby.h
        src/server/lobby/LaunchOptions.cpp
        src/server/lobby/LaunchOptions.h
        src/server/lobby/PlayerSlot.cpp
        src/server/lobby/PlayerSlot.h
        src/server/network/LobbyHandler.cpp
        src/server/network/LobbyHandler.h
        src/server/lobby/launch_ai.cpp
        src/server/lobby/launch_ai.h

        src/server/network/ServerMessageHandler.cpp
        src/server/network/ServerMessageHandler.h
        src/server/network/sockets_server.cpp
        src/server/network/ServerGameHandler.cpp
        src/server/network/ServerGameHandler.h

        src/server/sim/Engine.cpp
        src/server/sim/Engine.h
        src/server/network/server_game_messages.cpp
        src/server/network/server_game_messages.h
        src/server/sim/generate_map.cpp
        src/server/sim/generate_map.h
        src/server/network/ServerGameHandler.cpp
        src/server/network/ServerGameHandler.h
        src/server/sim/ServerGameState.cpp
        src/server/sim/ServerGameState.h
        src/server/sim/IncomingRequests.cpp
        src/server/sim/IncomingRequests.h

        src/server/actions/simulate.h
        src/server/actions/move.cpp
        src/server/actions/attack.cpp
        src/server/actions/idle.cpp
        src/server/actions/collect.cpp
        src/server/actions/deposit.cpp
        src/server/actions/strafe.cpp

        src/server/app/ServerSettings.cpp
        src/server/app/ServerSettings.h
)


add_executable(
        server

        src/server/app/server.cpp
)

target_link_libraries(
        server

        server_lib
        common_lib

        ${Boost_LIBRARIES}

        ${CMAKE_DL_LIBS}
        ${Backtrace_LIBRARY}

        pthread
        backtrace
)

