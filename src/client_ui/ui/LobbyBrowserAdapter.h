//
// Created by thallock on 2/15/20.
//

#ifndef PLAYING_LOBBYBROWSERADAPTER_H
#define PLAYING_LOBBYBROWSERADAPTER_H


#include <lobbybrowserhandler.h>

namespace aod::client::app { class UiClientContext; }

namespace aod::client::ui {

class LobbyBrowserAdapter : public LobbyBrowserHandler {
private:
	aod::client::app::UiClientContext& context;

public:
	LobbyBrowserAdapter(aod::client::app::UiClientContext& context) : context{context} {};

	void createLobby(const std::string &lobbyName);

	void refreshLobbies();

	void connect(const std::string &lobbyName);

	void disconnect();

	void setReady(bool ready);

	void launch();

	void requestStatusUpdate();

	void setSpectator(bool b);

	void setNumPlayers(int i);

	void setPlayerAsAi(int i, bool b, int type);
};

}

#endif //PLAYING_LOBBYBROWSERADAPTER_H
