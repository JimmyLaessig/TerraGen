#ifndef TERRAIN_H
#define TERRAIN_H

#include "transform.h"
#include <QOpenGLFunctions_4_4_Core>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QOpenGLTexture>
#include <QImage>

struct Grid
{
    int gridSize;
    float* vertices;
    float* uvs;

    unsigned int* indices;

    unsigned int numVertices;
    unsigned int numIndices;
};

class Terrain
{
public:

    Terrain(QOpenGLFunctions_4_4_Core* functions);
    virtual ~Terrain();

    float maxHeight = 1.0f;
    float texcoordScale = 1.0f;

    QOpenGLTexture* heightmapTexture;

    QOpenGLTexture* texture;

    std::vector<Transform> transforms;
    std::vector<glm::vec2> gridCoords;
    void drawTesselate();

    void drawSimple();

    void setHeightmapTexture(QImage* heightmapImage);

    void setGrid(Grid grid);

    void destroyGrid();

    int getGridRepetitionX();

    int getGridRepetitionY();

    void setGridRepetitionX(int value);

    void setGridRepetitionY(int value);

private:

    Grid grid;

    void setGridRepetitions(int x, int y);

    int gridRepetitionX = 1;
    int gridRepetitionY = 1;

    void createVAO();

    QOpenGLFunctions_4_4_Core* functions;

    GLuint terrainVAO;
    GLuint indexBuffer;
    GLuint vertexBuffer;
    GLuint barycentricCoordinatesBuffer;
    GLuint uvBuffer;
};


#endif // TERRAIN_H
