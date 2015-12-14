#ifndef TERRAIN_H
#define TERRAIN_H

#include "sceneobject.h"
#include <QOpenGLFunctions_4_4_Core>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QOpenGLTexture>
#include <QImage>

struct Geometry
{
    float* vertices;
    float* uvs;

    unsigned int* indices;

    unsigned int numVertices;
    unsigned int numIndices;

};

class Terrain : public SceneObject
{
public:

    Terrain(QOpenGLFunctions_4_4_Core* functions);
    virtual ~Terrain();

    float heightScale = 1.0f;
    float texcoordScale = 1.0f;

    QOpenGLTexture* heightmapTexture;

    QOpenGLTexture* texture;


    void drawTesselate();

    void drawSimple();

    void setHeightmapTexture(QImage* heightmapImage);

    void setGeometry(Geometry geometry);

    void createVAO();

private:
    Geometry geometry;

    QOpenGLFunctions_4_4_Core* functions;

    GLuint terrainVAO;
    GLuint indexBuffer;
    GLuint vertexBuffer;
    GLuint barycentricCoordinatesBuffer;
    GLuint uvBuffer;
};


#endif // TERRAIN_H
