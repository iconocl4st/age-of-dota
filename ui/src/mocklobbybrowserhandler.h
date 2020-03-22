#ifndef MOCKLOBBYBROWSERHANDLER_H
#define MOCKLOBBYBROWSERHANDLER_H

#include "lobbybrowserhandler.h"

class MockLobbyBrowserHandler : public LobbyBrowserHandler
{
public:
    MockLobbyBrowserHandler();

    void refreshLobbies();
    void connect(const std::string& lobbyName);
    void disconnect();
    void setReady(bool ready);
    void launch();
    void requestStatusUpdate();
};

#endif // MOCKLOBBYBROWSERHANDLER_H
