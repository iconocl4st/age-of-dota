#ifndef LOBBYBROWSERCONNECTION_H
#define LOBBYBROWSERCONNECTION_H

#include <string>

class LobbyBrowserConnection
{
public:
    bool connected{false};
    bool ready{false};
    bool allReady{false};
    std::string lobbyName{""};

    LobbyBrowserConnection();
};

#endif // LOBBYBROWSERCONNECTION_H
