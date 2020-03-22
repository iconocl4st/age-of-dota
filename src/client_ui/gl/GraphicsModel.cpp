//
// Created by thallock on 2/5/20.
//



#include "GraphicsModel.h"


#define VERTEX_ATTRIBUTE_NUMBER 0
#define TEXTURE_ATTRIBUTE_NUMBER 1

GraphicsModel::~GraphicsModel() {
    QOpenGLExtraFunctions *f = QOpenGLContext::currentContext()->extraFunctions();

    f->glDeleteVertexArrays(1, &vaoId);
    f->glDeleteBuffers(1, &vboId);
    f->glDeleteBuffers(1, &iboId);
}


GraphicsModel *createGraphicsModel(
        QOpenGLExtraFunctions *f,
        int numVertices, GLfloat *vertices,
        int numIndices, GLbyte *indices,
        GLfloat *textureCoords
) {
    auto *model = new GraphicsModel;
    f->glGenVertexArrays(1, &model->vaoId);
    f->glBindVertexArray(model->vaoId);

    model->vertexCount = numVertices;
    f->glGenBuffers(1, &model->vboId);
    f->glBindBuffer(GL_ARRAY_BUFFER, model->vboId);
    f->glBufferData(GL_ARRAY_BUFFER, 3 * numVertices * sizeof(*vertices), vertices, GL_STATIC_DRAW);
    f->glVertexAttribPointer(VERTEX_ATTRIBUTE_NUMBER, 3, GL_FLOAT, false, 0, nullptr);
    f->glBindBuffer(GL_ARRAY_BUFFER, 0);

    f->glGenBuffers(1, &model->tboId);
    f->glBindBuffer(GL_ARRAY_BUFFER, model->tboId);
    f->glBufferData(GL_ARRAY_BUFFER, 2 * numVertices * sizeof(*textureCoords), textureCoords, GL_STATIC_DRAW);
    f->glVertexAttribPointer(TEXTURE_ATTRIBUTE_NUMBER, 2, GL_FLOAT, false, 0, nullptr);
    f->glBindBuffer(GL_ARRAY_BUFFER, 0);

    model->indexCount = numIndices;
    f->glGenBuffers(1, &model->iboId);
    f->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, model->iboId);
    f->glBufferData(GL_ELEMENT_ARRAY_BUFFER, numIndices * sizeof(*indices), indices, GL_STATIC_DRAW);
    f->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    f->glBindVertexArray(0);

    return model;
}

void GraphicsModel::render(QOpenGLExtraFunctions *f) {
    f->glBindVertexArray(vaoId);
    f->glEnableVertexAttribArray(VERTEX_ATTRIBUTE_NUMBER);
    f->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboId);

    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_BYTE, nullptr);

    f->glDisableVertexAttribArray(VERTEX_ATTRIBUTE_NUMBER);
    f->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    f->glBindVertexArray(0);
}

