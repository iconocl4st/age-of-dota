
set(Boost_USE_STATIC_LIBS ON)


add_library(client_script_lib STATIC

        src/client_script/ScriptClientContext.cpp
        src/client_script/ScriptClientContext.h
        src/client_script/navigate_lobby.cpp
        src/client_script/navigate_lobby.h
        src/client_script/run_script_ai.cpp
        src/client_script/run_script_ai.h
        src/client_script/handle_game_messages.cpp
        src/client_script/handle_game_messages.h
        src/client_script/ScriptAi.cpp
        src/client_script/ScriptAi.h
        src/client_script/AiGroup.cpp
        src/client_script/AiGroup.h
        src/client_script/RandomMovementAiGroup.cpp
        src/client_script/RandomMovementAiGroup.h
)

add_executable(
        client_script

        src/client_script/script_client.cpp
)


target_link_libraries(
        client_script

        client_script_lib
        common_client_lib
        common_lib

        ${Boost_LIBRARIES}

        ${CMAKE_DL_LIBS}
        ${Backtrace_LIBRARY}

        pthread
        backtrace
)

