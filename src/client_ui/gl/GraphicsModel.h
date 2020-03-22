//
// Created by thallock on 2/5/20.
//

#ifndef PLAYING_GRAPHICSMODEL_H
#define PLAYING_GRAPHICSMODEL_H


#include <QOpenGLFunctions>
#include <QOpenGLExtraFunctions>

class GraphicsModel {
public:
    int vertexCount;
    GLuint vaoId;
    GLuint vboId;
    GLuint tboId;

    int indexCount;
    GLuint iboId;

    ~GraphicsModel();

    void render(QOpenGLExtraFunctions *f);
};


GraphicsModel *createGraphicsModel(
    QOpenGLExtraFunctions *f,
    int numVertices, GLfloat *vertices,
    int numIndices, GLbyte *indices,
    GLfloat *textureCoords
);



#endif //PLAYING_GRAPHICSMODEL_H
