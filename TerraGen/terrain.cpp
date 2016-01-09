#include "terrain.h"

#include <stdlib.h>

Terrain::Terrain(QOpenGLFunctions_4_4_Core* functions)
{
    this->functions = functions;
    heightmapTexture = nullptr;
    maxHeight = 1.0;

    colorTexture = new QOpenGLTexture(QImage("../Assets/test2.png"));

    setGridRepetitions(gridRepetitionX, gridRepetitionY);
}

Terrain::~Terrain()
{
    heightmapTexture->destroy();
    delete heightmapTexture;

    colorTexture->destroy();
    delete colorTexture;

    destroyGrid();
}


void Terrain::drawTesselate()
{
    functions->glPatchParameteri(GL_PATCH_VERTICES, 3);
    functions->glBindVertexArray(terrainVAO);
    functions->glDrawElements(GL_PATCHES, grid.numIndices, GL_UNSIGNED_INT, 0);
    functions->glBindVertexArray(0);
}


void Terrain::drawSimple()
{
    functions->glBindVertexArray(terrainVAO);
    functions->glDrawElements(GL_TRIANGLES, grid.numIndices, GL_UNSIGNED_INT, 0);
    functions->glBindVertexArray(0);
}


void Terrain::setHeightmapTexture(QImage *heightmapImage)
{
    // Destroy old Texture if available
    if(heightmapTexture != nullptr)
    {
        heightmapTexture->destroy();
        delete this->heightmapTexture;
    }

    heightmapTexture = new QOpenGLTexture(*heightmapImage);
}

void Terrain::setColorTexture(QImage *colorImage)
{
    // Destroy old Texture if available
    if(colorTexture != nullptr)
    {
        colorTexture->destroy();
        delete this->colorTexture;
    }

    colorTexture = new QOpenGLTexture(*colorImage);
}


void Terrain::setGridRepetitionX(int value)
{
    setGridRepetitions(value, gridRepetitionY);
}

void Terrain::setGridRepetitionY(int value)
{
    setGridRepetitions(gridRepetitionX, value);
}

int Terrain::getGridRepetitionX()
{
    return gridRepetitionX;
}

int Terrain::getGridRepetitionY()
{
    return gridRepetitionY;
}

void Terrain::setGridRepetitions(int x, int y)
{
    gridRepetitionX = x;
    gridRepetitionY = y;

    float offsetX = -(float)((grid.gridSize) * (x-1)) / 2.0f;
    float offsetZ = (float)((grid.gridSize) * (y-1)) / 2.0f;

    qDebug("Grid Repetitions: %d, %d", gridRepetitionX, gridRepetitionY);
    qDebug("OffsetX: %f", offsetX);
    transforms.clear();
    gridCoords.clear();

    for(int i = 0; i < gridRepetitionX; i++)
    {
        for(int j = 0; j < gridRepetitionY; j++)
        {
            int translateX = grid.gridSize *  i + offsetX;
            int translateZ = -grid.gridSize *  j + offsetZ;

            Transform t;
            t.translate(glm::vec3(translateX, 0, translateZ));
            transforms.push_back(t);
            gridCoords.push_back(glm::vec2(i, j));
        }
    }
}

void Terrain::setGrid(Grid grid)
{
    // Destroy old grid
    destroyGrid();

    // Set new gird
    this->grid = grid;

    // Create Grid on GPU
    createVAO();
}

void Terrain::destroyGrid()
{
    if(functions)
    {
        if (indexBuffer)  functions->glDeleteBuffers(1, &indexBuffer);
        if (vertexBuffer) functions->glDeleteBuffers(1, &vertexBuffer);
        if (uvBuffer)     functions->glDeleteBuffers(1, &uvBuffer);
        if (terrainVAO)   functions->glDeleteVertexArrays(1, &terrainVAO);
    }
}


void Terrain::createVAO()
{
    // Bind VAO
    functions->glGenVertexArrays(1, &terrainVAO);
    functions->glBindVertexArray(terrainVAO);

    // Buffer for the indices
    functions->glGenBuffers(1, &indexBuffer);
    functions->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
    functions->glBufferData(GL_ELEMENT_ARRAY_BUFFER, grid.numIndices * sizeof(unsigned int), grid.indices, GL_STATIC_DRAW);
    qDebug("Created IndexBuffer");

    // Buffer for the vertices
    functions->glGenBuffers(1, &vertexBuffer);
    functions->glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    functions->glBufferData(GL_ARRAY_BUFFER, grid.numVertices * 3 * sizeof(float), grid.vertices, GL_STATIC_DRAW);

    functions->glEnableVertexAttribArray(0);
    functions->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    qDebug("Created vertexbuffer");

    // Buffer for the uvs
    functions->glGenBuffers(1, &uvBuffer);
    functions->glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
    functions->glBufferData(GL_ARRAY_BUFFER, grid.numVertices * 2 * sizeof(float), grid.uvs, GL_STATIC_DRAW);

    functions->glEnableVertexAttribArray(1);
    functions->glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
    qDebug("Created uvBuffer");

    // Release VAO
    functions->glBindVertexArray(0);
}

