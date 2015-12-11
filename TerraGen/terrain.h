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

    void setGeometry(float* vertices, int numVertices);
    void createVAO();

private:

    float* vertices;
    int numVertices;

    QOpenGLFunctions_4_4_Core* functions;

    GLuint terrainVAO;
    GLuint vertexBuffer;
    GLuint uvBuffer;
};

#endif // TERRAIN_H
