//
// Created by thallock on 2/6/20.
//

#include "ScreenMap.h"

#include <GL/glu.h>

#include <math.h>

void ScreenMap::initialize() {
    glGetIntegerv(GL_VIEWPORT, &viewport[0]);
    glGetDoublev(GL_MODELVIEW_MATRIX, &mvmatrix[0]);
    glGetDoublev(GL_PROJECTION_MATRIX, &projmatrix[0]);
}

void ScreenMap::setWCoords(GLdouble x, GLdouble y) {
    GLdouble realy = y; // viewport[3] - y - 1;
    gluUnProject(
            x, realy, 0,
            &mvmatrix[0], &projmatrix[0], &viewport[0],
            &wcoord1[0], &wcoord1[1], &wcoord1[2]
    );
    gluUnProject(
            x, realy, 1,
            &mvmatrix[0], &projmatrix[0], &viewport[0],
            &wcoord2[0], &wcoord2[1], &wcoord2[2]
    );
}

void ScreenMap::setRay(GLdouble x, GLdouble y) {
    setWCoords(x, y);
    gameRayX = wcoord2[0] - wcoord1[0];
    gameRayY = wcoord2[1] - wcoord1[1];
    gameRayZ = wcoord2[2] - wcoord1[2];
    GLdouble n = sqrt(gameRayX * gameRayX + gameRayY * gameRayY + gameRayZ * gameRayZ);
    gameRayX /= n;
    gameRayY /= n;
    gameRayZ /= n;
}

void ScreenMap::map(GLdouble x, GLdouble y, GLdouble desiredZ) {
    setWCoords(x, y);
    double t = (desiredZ - wcoord1[2]) / (wcoord2[2] - wcoord1[2]);
    gameLocationX = wcoord1[0] + (wcoord2[0] - wcoord1[0]) * t;
    gameLocationY = wcoord1[1] + (wcoord2[1] - wcoord1[1]) * t;
}
