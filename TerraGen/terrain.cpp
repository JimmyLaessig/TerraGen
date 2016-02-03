#include "terrain.h"

#include <stdlib.h>

Terrain::Terrain(QOpenGLFunctions_4_4_Core* functions)
{
    this->functions = functions;
    heightmapTexture = nullptr;
    maxHeight = 1.0;

    grassTexture = new QOpenGLTexture(QImage("Assets/GrassGreenTexture0003.jpg"));
    rockTexture = new QOpenGLTexture(QImage("Assets/rock.jpg"));

    setGridRepetitions(gridRepetitionX, gridRepetitionY);
}

Terrain::~Terrain()
{
    heightmapTexture->destroy();
    delete heightmapTexture;

    grassTexture->destroy();
    delete grassTexture;

    destroyGrid();
}


void Terrain::drawTesselate()
{
     functions->glPatchParameteri(GL_PATCH_VERTICES, 3);
     functions->glBindVertexArray(grid.terrainVAO);
     functions->glDrawElements(GL_PATCHES, grid.numIndices, GL_UNSIGNED_INT, 0);
     functions->glBindVertexArray(0);
}


void Terrain::drawSimple()
{
     functions->glBindVertexArray(grid.terrainVAO);
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

    tiles.clear();
    tileCoordinates.clear();

    for(int i = 0; i < gridRepetitionX; i++)
    {
        for(int j = 0; j < gridRepetitionY; j++)
        {
            float translateX = grid.gridSize * i;
            float translateZ =-grid.gridSize * j;

            Transform t;
            t.translate(glm::vec3(translateX, 0, translateZ));
            tiles.push_back(t);
            glm::vec2 tileCoords = glm::vec2(i,j);
            tileCoordinates.push_back(tileCoords);
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
        if (grid.indexBuffer)   functions->glDeleteBuffers(1, &grid.indexBuffer);
        if (grid.vertexBuffer)  functions->glDeleteBuffers(1, &grid.vertexBuffer);
        if (grid.uvBuffer)      functions->glDeleteBuffers(1, &grid.uvBuffer);
        if (grid.terrainVAO)    functions->glDeleteVertexArrays(1, &grid.terrainVAO);
    }
}


void Terrain::createVAO()
{
    // Bind VAO
     functions->glGenVertexArrays(1, &grid.terrainVAO);
     functions->glBindVertexArray(grid.terrainVAO);

    // Buffer for the indices
     functions->glGenBuffers(1, &grid.indexBuffer);
     functions->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, grid.indexBuffer);
     functions->glBufferData(GL_ELEMENT_ARRAY_BUFFER, grid.numIndices * sizeof(unsigned int), grid.indices, GL_STATIC_DRAW);
    qDebug("Created IndexBuffer");

    // Buffer for the vertices
     functions->glGenBuffers(1, &grid.vertexBuffer);
     functions->glBindBuffer(GL_ARRAY_BUFFER, grid.vertexBuffer);
     functions->glBufferData(GL_ARRAY_BUFFER, grid.numVertices * 3 * sizeof(float), grid.vertices, GL_STATIC_DRAW);

     functions->glEnableVertexAttribArray(0);
    functions->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    qDebug("Created vertexbuffer");

    // Buffer for the uvs
     functions->glGenBuffers(1, &grid.uvBuffer);
     functions->glBindBuffer(GL_ARRAY_BUFFER, grid.uvBuffer);
     functions->glBufferData(GL_ARRAY_BUFFER, grid.numVertices * 2 * sizeof(float), grid.uvs, GL_STATIC_DRAW);

     functions->glEnableVertexAttribArray(1);
     functions->glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
    qDebug("Created uvBuffer");

    // Release VAO
     functions->glBindVertexArray(0);
}

