#include "terrain.h"
#include <stdlib.h>

Terrain::Terrain(QOpenGLFunctions_4_4_Core* functions): SceneObject()
{
    this->functions = functions;
    noiseTexture = nullptr;
    texture = new QOpenGLTexture(QImage("../Assets/test2.png"));
}

Terrain::~Terrain()
{
    noiseTexture->destroy();
    delete noiseTexture;
    texture->destroy();
    delete texture;
    functions->glDeleteBuffers(1, &indexBuffer);
    functions->glDeleteBuffers(1, &vertexBuffer);
    functions->glDeleteBuffers(1, &uvBuffer);
    functions->glDeleteVertexArrays(1, &terrainVAO);
}

void Terrain::drawTesselate()
{
    functions->glPatchParameteri(GL_PATCH_VERTICES, 3);
    functions->glBindVertexArray(terrainVAO);
    functions->glDrawElements(GL_PATCHES, geometry.numIndices, GL_UNSIGNED_INT, 0);
    functions->glBindVertexArray(0);
}


void Terrain::drawSimple()
{
    functions->glBindVertexArray(terrainVAO);
    functions->glDrawElements(GL_TRIANGLES, geometry.numIndices, GL_UNSIGNED_INT, 0);
    functions->glBindVertexArray(0);
}


void Terrain::setNoiseTexture(QImage *noiseImage)
{
    // Destroy old Texture if available
    if(this->noiseTexture != nullptr)
    {
     noiseTexture->destroy();
        delete this->noiseTexture;
    }

    this->noiseTexture = new QOpenGLTexture(*noiseImage);
}

void Terrain::setGeometry(Geometry geometry)
{
    this->geometry = geometry;
}

void Terrain::createVAO()
{
    // Bind VAO
    functions->glGenVertexArrays(1, &terrainVAO);
    functions->glBindVertexArray(terrainVAO);

    // Buffer for the indices
    functions->glGenBuffers(1, &indexBuffer);
    functions->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
    functions->glBufferData(GL_ELEMENT_ARRAY_BUFFER, geometry.numIndices * sizeof(unsigned int), geometry.indices, GL_STATIC_DRAW);

    // Buffer for the vertices
    functions->glGenBuffers(1, &vertexBuffer);
    functions->glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    functions->glBufferData(GL_ARRAY_BUFFER, geometry.numVertices * 3 * sizeof(float), geometry.vertices, GL_STATIC_DRAW);

    functions->glEnableVertexAttribArray(0);
    functions->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

    // Buffer for the uvs
    functions->glGenBuffers(1, &uvBuffer);
    functions->glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
    functions->glBufferData(GL_ARRAY_BUFFER, geometry.numVertices * 2 * sizeof(float), geometry.uvs, GL_STATIC_DRAW);

    functions->glEnableVertexAttribArray(1);
    functions->glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);

    // Release VAO
    functions->glBindVertexArray(0);
}

