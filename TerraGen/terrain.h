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

    void setNoiseTexture(QImage* noiseImage);

    float height;
    float width;

    float texcoordScale = 1.0f;

    Terrain(QOpenGLFunctions_4_4_Core* functions);
    virtual ~Terrain();

    void drawTesselate();

    void drawSimple();


    QOpenGLTexture* noiseTexture;

    QOpenGLTexture* texture;

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
