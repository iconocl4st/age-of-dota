//
// Created by thallock on 2/6/20.
//

#include <iostream>
#include <memory>
#include <fstream>

#include <thread>
#include <QApplication>
#include <QDebug>

#include <exception>

#include "UiClientContext.h"
#include "../ui/LobbyBrowserAdapter.h"
#include "../network/ClientMessageHandler.h"
#include "../../common/network/sockets.h"
#include "../../common/json/JsonSplitWriter.h"
#include "../../common/json/JsonTextWriter.h"
#include "../../common/json/JsonTextReader.h"
#include "../../common/network/CloseOnExit.h"
#include "../../common/network/DefaultMessageHandler.h"
#include "../../common/network/handle_messages.h"
#include "../../common/network/send_noop.h"
#include "../../common/util/Timer.h"


namespace aod::client::app {

ConnectionContext *UiClientContext::getConnection() {
	std::unique_lock<std::recursive_mutex> lock{connectionMutex};

	if (connectionContext == nullptr) {
		throw std::runtime_error("No connection context yet");
	}
	return connectionContext;
}

void UiClientContext::runConnection(const std::string &address, int port) {
	int socket;

	connectionMutex.lock();
	if (connectionContext != nullptr) {
		return;
	}

	socket = createClientSocket(address, port);
	CloseOnExit coe{socket};

	BOOST_LOG_SEV(logger, NORMAL) << "Ui client connected";

	std::ostringstream debugPath;
	debugPath << settings.outputDirectory << "/ui_client_debug.json";
	std::ofstream debugFile;
	debugFile.open(debugPath.str());

	SocketOutputBufferWrapper socketBuffer{socket};
	TextJsonWriter socketWriter{socketBuffer};
	StreamOutputBufferWrapper streamBuffer{debugFile};
	TextJsonWriter streamWriter{streamBuffer};
	JsonSplitWriter writer{socketWriter, streamWriter};

	ConnectionContext cc{"Client", writer};
	sendNoop(cc);

	{
		std::unique_lock<std::recursive_mutex> lock{connectionMutex};
		connectionContext = &cc;
	}

	if (onConnect) {
		onConnect.value()(*this);
	}

	std::list<MessageHandler *> handlers;
	DefaultMessageHandler dh; handlers.push_back(&dh);
	ClientMessageHandler ch{*this}; handlers.push_back(&ch);

	SocketInputBufferWrapper readBuffer{socket};
	TextJsonReader reader{readBuffer, "ui-client-reader"};
	MessageHandlingArgs handlingContext{reader, *connectionContext, logger};
	while (!handlingContext.stopHandlingMessages) {
		handleNextMessage(handlers, handlingContext);
	}

	{
		std::unique_lock<std::recursive_mutex> lock{connectionMutex};
		connectionContext = nullptr;
	}
}


void UiClientContext::closeGame() {
	{
		std::unique_lock<std::recursive_mutex> lock{gameMutex};
		if (clientGameState == nullptr) {
			throw std::runtime_error("Not in a game.");
		}

		delete clientGameState;
		clientGameState = nullptr;

		lobbyBrowser.hide();
		gameWindow.hide();

		gameAdapter.deInitialize();
	}

	renderThread->join();

	{
		std::unique_lock<std::recursive_mutex> lock{gameMutex};
		delete renderThread;
		renderThread = nullptr;
	}

	if (onCloseGame) {
		onCloseGame.value()(*this);
	}
}

void UiClientContext::launchGame(PlayerLaunchOptions &options) {
	std::unique_lock<std::recursive_mutex> lock{gameMutex};
	if (clientGameState != nullptr) {
		throw std::runtime_error("Already in a game.");
	}

	clientGameState = new UiClientGameState{options};
	clientGameState->clientGameState.sharedState.specification = options.spec;

	lobbyBrowser.hide();
//	gameWindow.show();
	gameWindow.showFullScreen();

	gameAdapter.initializeModels();
	gameAdapter.repaint();

	gameAdapter.center(options.spec->width / 2, options.spec->height / 2);

	// if it wasn't null?
	renderThread = new std::thread{[this]() -> void { renderLoop(); }};
}

void UiClientContext::renderLoop() {
	Timer timer{std::chrono::milliseconds{20}};

	while (isInGame()) {
		gameWindow.repaint();
		timer.waitForNextTick();
	}
}

int runClient(int argc, char **argv,
	void (*onConnect)(UiClientContext& context),
	void (*onReady)(UiClientContext& context),
	void (*onClose)(UiClientContext& context)
) {
	qDebug() << QT_VERSION_STR;

	QApplication application(argc, argv);

	UiClientContext context;
	BOOST_LOG_SEV(context.logger, NORMAL) << "Launching UI client.";

	aod::client::common::loadSettings("./settings/client_settings.json", context.settings);

	context.lobbyBrowser.show();

	// TODO
	context.gameWindow.show();
	context.gameWindow.hide();

	context.onConnect = onConnect;
	context.onReady = onReady;
	context.onCloseGame = onClose;

	std::thread connectionThread{[&context]() -> void {
		context.runConnection(context.settings.serverAddress, context.settings.port);
	}};

	int ret = application.exec();

	connectionThread.join();

	return ret;
}

}
























//void UiClientContext::runScheduled() {
//	std::unique_lock<std::recursive_mutex> lock{scheduledMutex};
//
//	std::list<ScheduledTask *> dueNow;
//
//	for (auto &scheduled_task : scheduled) {
//		if (scheduled_task->condition()) {
//			dueNow.push_back(scheduled_task);
//		}
//	}
//
//	for (auto &scheduled_task : dueNow) {
//		scheduled.remove(scheduled_task);
//	}
//
//	for (auto &scheduled_task : dueNow) {
//		scheduled_task->toRun();
//	}
//
//	for (auto &scheduled_task : dueNow) {
//		delete scheduled_task;
//	}
//}

//void UiClientContext::schedule(ScheduledTask *task) {
//	std::unique_lock<std::recursive_mutex> lock{scheduledMutex};
//	scheduled.push_back(task);
//}

//class ScheduledTask {
//public:
//	std::function<bool(void)> condition;
//	std::function<void(void)> toRun;
//};

//	std::recursive_mutex scheduledMutex;
//	std::list<ScheduledTask *> scheduled{};

//	void runScheduled();
//	void schedule(ScheduledTask *task);