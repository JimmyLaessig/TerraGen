#include "terrain.h"


Terrain::Terrain(QOpenGLFunctions_4_4_Core* functions): SceneObject()
{
    this->functions = functions;
}

Terrain::~Terrain()
{

}

void Terrain::drawTesselate()
{
    functions->glPatchParameteri(GL_PATCH_VERTICES, 3);
    functions->glBindVertexArray(terrainVAO);
    functions->glDrawElements(GL_PATCHES, numIndices, GL_UNSIGNED_INT, 0);
    functions->glBindVertexArray(0);
}

void Terrain::drawGrid()
{
    functions->glBindVertexArray(terrainVAO);
    functions->glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, 0);
    functions->glBindVertexArray(0);
}

void Terrain::setGeometry(float* vertices, int numVertices, unsigned int* indices, int numIndices)
{
    this->vertices = vertices;
    this->numVertices = numVertices;
    this->indices = indices;
    this->numIndices = numIndices;
}

void Terrain::createVAO()
{

    float uvs[] = {
        // Triangle 1
        0.0f ,0.0f,
        0.0f ,1.0f,
        1.0f ,0.0f,
        // Triangle 2
        0.0f ,1.0f,
        1.0f ,0.0f,
        1.0f ,1.0f
    };

    functions->glGenVertexArrays(1, &terrainVAO);
    functions->glBindVertexArray(terrainVAO);

    // Buffer for the indices
    functions->glGenBuffers(1, &indexBuffer);
    functions->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
    functions->glBufferData(GL_ELEMENT_ARRAY_BUFFER, numIndices * sizeof(unsigned int), indices, GL_STATIC_DRAW);


    // Buffer for the vertices
    functions->glGenBuffers(1, &vertexBuffer);
    functions->glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    functions->glBufferData(GL_ARRAY_BUFFER, numVertices * 3 * sizeof(float), vertices, GL_STATIC_DRAW);

    functions->glEnableVertexAttribArray(0);
    functions->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);


    qDebug("Generate uvVBO");
    // Buffer for the uvs

    //    functions->glGenBuffers(1, &uvBuffer);
    //    functions->glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
    //    functions->glBufferData(GL_ARRAY_BUFFER, numVertices * 2 * sizeof(float), uvs, GL_STATIC_DRAW);

    //    functions->glEnableVertexAttribArray(1);
    //    functions->glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
    functions->glBindVertexArray(0);
}

