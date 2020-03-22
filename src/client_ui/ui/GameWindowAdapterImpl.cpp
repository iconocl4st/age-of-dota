//
// Created by thallock on 3/9/20.
//

#include "GameWindowAdapterImpl.h"


#include "../gl/Render.h"
#include "../app/UiClientContext.h"
#include "../gl/GraphicsModel.h"

#include <iostream>

#include <QOpenGLExtraFunctions>

#include <GL/glu.h>

namespace aod::client::graphics {


GameWindowAdapterImpl::GameWindowAdapterImpl(aod::client::app::UiClientContext &clientContext) : clientContext{clientContext} {}


void _initializeModels(QOpenGLExtraFunctions *f) {
	GLfloat vertices[] = {
		-0.5f, 0.5f, 0.0f,
		-0.5f, -0.5f, 0.0f,
		0.5f, 0.5f, 0.0f,
		0.5f, -0.5f, 0.0f
	};

	GLbyte vertexIndices[] = {0, 1, 2, 2, 1, 3};

	GLfloat textureCoords[] = {
		0, 1,
		0, 0,
		1, 1,
		1, 0
	};

	GraphicsModel *model = createGraphicsModel(
		f,
		4, &vertices[0],
		6, &vertexIndices[0],
		&textureCoords[0]
	);

	delete model;
}


void GameWindowAdapterImpl::initializeGL() {
	QOpenGLExtraFunctions *f = QOpenGLContext::currentContext()->extraFunctions();
}

void GameWindowAdapterImpl::paintGL() {
	QOpenGLExtraFunctions *f = QOpenGLContext::currentContext()->extraFunctions();

	if (initialize) {
		_initializeModels(f);
		initialize = false;
	}

	glClear(GL_COLOR_BUFFER_BIT);
	glClearColor(0, 0, 0, 0);

	glLoadIdentity();
//    gluLookAt(
//            context->cameraInfo.locationX,
//            context->cameraInfo.locationY,
//            context->cameraInfo.locationZ,
//            0, 0, 0,
//            0, 1, 0
//    );

	glTranslatef(-cameraInfo.locationX, -cameraInfo.locationY, cameraInfo.locationZ);

	screenMap.initialize();
	updateScreenLocation();

	render(clientContext, f);
}

void GameWindowAdapterImpl::resizeGL(int width, int height) {
	glClear(GL_COLOR_BUFFER_BIT);
	glClearColor(0, 0, 0, 0);

	if (height == 0) height = 1;

	cameraInfo.aspect = width / (float) height;
	cameraInfo.screenWidth = width;
	cameraInfo.screenHeight = height;

	glViewport(0, 0, width, height);

	// Setup perspective projection, with aspect ratio matches viewport
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(FOV_Y, cameraInfo.aspect, Z_NEAR, Z_FAR);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void GameWindowAdapterImpl::keyPressEvent() {
}


void GameWindowAdapterImpl::mouseMoveEvent(int x, int y) {
	dragListener.draggedTo(x, y);
	repaint();
}

void GameWindowAdapterImpl::updateScreenLocation() {
	screenMap.map(0, 0);
	cameraInfo.screenLowerX = screenMap.gameLocationX;
	cameraInfo.screenUpperY = screenMap.gameLocationY;

	screenMap.map(cameraInfo.screenWidth, cameraInfo.screenHeight);
	cameraInfo.screenUpperX = screenMap.gameLocationX;
	cameraInfo.screenLowerY = screenMap.gameLocationY;

	cameraInfo.visibleHeight = cameraInfo.screenUpperY - cameraInfo.screenLowerY;
	cameraInfo.visibleWidth = cameraInfo.screenUpperX - cameraInfo.screenLowerX;
	cameraInfo.pixelsPerTileY = cameraInfo.screenHeight / cameraInfo.visibleHeight;
	cameraInfo.pixelsPerTileX = cameraInfo.screenWidth / cameraInfo.visibleWidth;
}

void GameWindowAdapterImpl::zoom(double factor, double x, double y) {
	screenMap.setRay(x, y);

	GLdouble desiredZ = cameraInfo.locationZ * factor;
	GLdouble t = (desiredZ - cameraInfo.locationZ) / screenMap.gameRayZ;

	cameraInfo.locationX -= (float) (t * screenMap.gameRayX);
	cameraInfo.locationY += (float) (t * screenMap.gameRayY);
	cameraInfo.locationZ += (float) (t * screenMap.gameRayZ);
}

void GameWindowAdapterImpl::mouseWheel(int amount, int x, int y) {
	if (amount == 0) {
		return;
	}
	if (amount > 0) {
		zoom(ZOOM_FACTOR, x, y);
	} else {
		zoom(1.0 / ZOOM_FACTOR, x, y);
	}
	repaint();
}

void GameWindowAdapterImpl::repaint() {
	clientContext.gameWindow.repaint();
}

void GameWindowAdapterImpl::mousePressEvent(int x, int y, int btn) {
	if (btn == MOUSE_MIDDLE_BUTTON) {
		dragListener.beginDragging(x, y);
	}
}

void GameWindowAdapterImpl::mouseReleaseEvent(int x, int y, int btn) {
	if (btn == MOUSE_MIDDLE_BUTTON) {
		dragListener.endDragging();
	}
}

void GameWindowAdapterImpl::initializeModels() {
	initialize = true;
}

void GameWindowAdapterImpl::deInitialize() {


}

void GameWindowAdapterImpl::center(double x, double y) {
	cameraInfo.locationX = x;
	cameraInfo.locationY = y;
}

}