

add_library(
        common_client_lib STATIC

        src/client_common/network/sockets_client.cpp
        src/common/network/sockets.h
        src/client_common/state/ClientGameState.cpp
        src/client_common/state/ClientGameState.h
        src/client_common/network/client_messages.cpp
        src/client_common/network/client_messages.h

        src/client_common/util/ClientSettings.cpp
        src/client_common/util/ClientSettings.h
)
