//
// Created by thallock on 2/6/20.
//

#ifndef PLAYING_SCREENMAP_H
#define PLAYING_SCREENMAP_H

#include <QOpenGLFunctions>

class ScreenMap {

private:
    GLint viewport[4];
    GLdouble mvmatrix[16];
    GLdouble projmatrix[16];

    // Could combine these into a single array...
    GLdouble wcoord1[3];
    GLdouble wcoord2[3];

    void setWCoords(GLdouble x, GLdouble y);
public:
    float gameLocationX;
    float gameLocationY;

    float gameRayX;
    float gameRayY;
    float gameRayZ;

    void initialize();
    void setRay(GLdouble x, GLdouble y);
    void map(GLdouble x, GLdouble y) { map(x, y, 0); }
    void map(GLdouble x, GLdouble y, GLdouble desiredZ);
};



#endif //PLAYING_SCREENMAP_H
