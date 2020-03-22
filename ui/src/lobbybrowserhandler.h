#ifndef LOBBYBROWSERHANDLER_H
#define LOBBYBROWSERHANDLER_H

#include <string>

class LobbyBrowserHandler
{
public:
    LobbyBrowserHandler();
    virtual ~LobbyBrowserHandler() = default;

    virtual void createLobby(const std::string& lobbyName) = 0;
    virtual void refreshLobbies() = 0;
    virtual void connect(const std::string& lobbyName) = 0;
    virtual void disconnect() = 0;
    virtual void setReady(bool ready) = 0;
    virtual void launch() = 0;
    virtual void requestStatusUpdate() = 0;
};

#endif // LOBBYBROWSERHANDLER_H
