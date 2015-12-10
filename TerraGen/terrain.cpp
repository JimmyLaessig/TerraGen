#include "terrain.h"


Terrain::Terrain(QOpenGLFunctions_4_4_Core* functions): SceneObject()
{
    this->functions = functions;
}

Terrain::~Terrain()
{

}

void Terrain::draw()
{
    functions->glPatchParameteri(GL_PATCH_VERTICES, 3);
    functions->glBindVertexArray(terrainVAO);
    functions->glDrawArrays(GL_PATCHES, 0, numVertices);
    functions->glBindVertexArray(0);
}


void Terrain::setGeometry(float* vertices, int numVertices)
{
    this->vertices = vertices;
    this->numVertices = numVertices;
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
    qDebug("Generate VAO");
    functions->glGenVertexArrays(1, &terrainVAO);
    functions->glBindVertexArray(terrainVAO);


    qDebug("Generate VertexVBO");
    // Buffer for the vertices

    functions->glGenBuffers(1, &vertexBuffer);
    functions->glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    functions->glBufferData(GL_ARRAY_BUFFER, numVertices * 3 * sizeof(float), vertices, GL_STATIC_DRAW);

    functions->glEnableVertexAttribArray(0);
    functions->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);


    qDebug("Generate uvVBO");
    // Buffer for the uvs

    functions->glGenBuffers(1, &uvBuffer);
    functions->glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
    functions->glBufferData(GL_ARRAY_BUFFER, numVertices * 2 * sizeof(float), uvs, GL_STATIC_DRAW);

    functions->glEnableVertexAttribArray(1);
    functions->glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);

}

