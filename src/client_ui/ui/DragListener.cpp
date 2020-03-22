//
// Created by thallock on 2/6/20.
//

#include "DragListener.h"

#include "../gl/CameraInfo.h"

void DragListener::beginDragging(int sx, int sy) {
    dragging = true;
    screenMouseX = (float) sx;
    screenMouseY = (float) sy;

    gameMouseX = cameraInfo.locationX;
    gameMouseY = cameraInfo.locationY;
}

void DragListener::draggedTo(int sx, int sy) {
    if (!dragging) {
        return;
    }

    double deltaX = ((float) sx - screenMouseX) / cameraInfo.pixelsPerTileX;
    double deltaY = ((float) sy - screenMouseY) / cameraInfo.pixelsPerTileY;
    cameraInfo.locationX = (float) (gameMouseX - deltaX);
    cameraInfo.locationY = (float) (gameMouseY - deltaY);
}

void DragListener::endDragging() {
    dragging = false;
    screenMouseX = -1;
    screenMouseY = -1;
    gameMouseX = -1;
    gameMouseY = -1;
}
