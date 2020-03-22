//
// Created by thallock on 3/9/20.
//

#ifndef PLAYING_GAMEWINDOWADAPTERIMPL_H
#define PLAYING_GAMEWINDOWADAPTERIMPL_H

#include <gamewindowadapter.h>


#include "../gl/CameraInfo.h"
#include "../gl/ScreenMap.h"
#include "DragListener.h"

namespace aod::client::app { class UiClientContext; }


namespace aod::client::graphics {

class GameWindowAdapterImpl : public GameWindowAdapter {
private:
	aod::client::app::UiClientContext& clientContext;

	CameraInfo cameraInfo{};
	ScreenMap screenMap{};
	DragListener dragListener{cameraInfo};
	bool initialize{false};

	void updateScreenLocation();

	void zoom(double factor, double x, double y);

public:
	GameWindowAdapterImpl(aod::client::app::UiClientContext& clientContext);

	void initializeGL();

	void paintGL();

	void resizeGL(int width, int height);

	void keyPressEvent();

	void mousePressEvent(int x, int y, int btn);

	void mouseReleaseEvent(int x, int y, int btn);

	void mouseMoveEvent(int x, int y);

	void mouseWheel(int amount, int x, int y);

	void initializeModels();
	void deInitialize();

	void repaint();

	void center(double x, double y);
};

}

#endif //PLAYING_GAMEWINDOWADAPTERIMPL_H
