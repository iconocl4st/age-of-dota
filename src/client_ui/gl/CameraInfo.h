//
// Created by thallock on 2/6/20.
//

#ifndef PLAYING_CAMERAINFO_H
#define PLAYING_CAMERAINFO_H


#include "GLConstants.h"

class CameraInfo {
public:
    float locationX;
    float locationY;
    float locationZ;

    float aspect;

    int screenWidth;
    int screenHeight;

    double screenLowerX;
    double screenUpperY;
    double screenUpperX;
    double screenLowerY;

    double visibleHeight;
    double visibleWidth;
    double pixelsPerTileY;
    double pixelsPerTileX;

    CameraInfo() :
        locationX{0},
        locationY{0},
        locationZ{INITIAL_Z},
        aspect{1},
        screenWidth{1},
        screenHeight{1},
        screenLowerX{0},
        screenUpperY{0},
        screenUpperX{0},
        screenLowerY{0},
        visibleHeight{0},
        visibleWidth{0},
        pixelsPerTileY{0},
        pixelsPerTileX{0}
    {}
};


#endif //PLAYING_CAMERAINFO_H
