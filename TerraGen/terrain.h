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

    void draw();

private:
	QOpenGLFunctions_4_4_Core* functions;

    void initVAO();
    GLuint terrainVAO;
    GLuint vertexBuffer;
    GLuint uvBuffer;
};

#endif // TERRAIN_H
