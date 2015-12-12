#ifndef TERRAIN_H
#define TERRAIN_H

#include "sceneobject.h"
#include <QOpenGLFunctions_4_4_Core>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>


class Terrain : public SceneObject
{
public:
    Terrain(QOpenGLFunctions_4_4_Core* functions);
    virtual ~Terrain();

    void drawTesselate();

    void drawGrid();

    void setGeometry(float* vertices, float *uvs, int numVertices, unsigned int* indices, int numIndices);
    void createVAO();

private:

    float* vertices;
    float* uvs;

    int numVertices;

    unsigned int* indices;
    int numIndices;

    QOpenGLFunctions_4_4_Core* functions;

    GLuint terrainVAO;
    GLuint indexBuffer;
    GLuint vertexBuffer;
    GLuint uvBuffer;
};

#endif // TERRAIN_H
