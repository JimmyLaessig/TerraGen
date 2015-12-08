#ifndef TERRAIN_H
#define TERRAIN_H

#include "sceneobject.h"
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>


class Terrain : public SceneObject
{
public:
    Terrain(QOpenGLFunctions_3_3_Core* functions);
    virtual ~Terrain();

    void draw();

private:
    QOpenGLFunctions_3_3_Core* functions;

    void initVAO(QOpenGLFunctions_3_3_Core* functions);
    GLuint terrainVAO;
    GLuint vertexBuffer;
    GLuint uvBuffer;
};

#endif // TERRAIN_H
