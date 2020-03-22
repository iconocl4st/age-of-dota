//
// Created by thallock on 2/6/20.
//

#ifndef PLAYING_DRAGLISTENER_H
#define PLAYING_DRAGLISTENER_H


#include "../gl/ScreenMap.h"

class CameraInfo;

class DragListener {
    bool dragging;
    float screenMouseX, screenMouseY;
    float gameMouseX, gameMouseY;

    CameraInfo& cameraInfo;
public:
    DragListener(CameraInfo& cameraInfo) :
        dragging{false},
        screenMouseX{-1},
        screenMouseY{-1},
        gameMouseX{-1},
        gameMouseY{-1},
        cameraInfo{cameraInfo} {}

    void beginDragging(int sx, int sy);
    void draggedTo(int sx, int sy);
    void endDragging();
};


#endif //PLAYING_DRAGLISTENER_H
