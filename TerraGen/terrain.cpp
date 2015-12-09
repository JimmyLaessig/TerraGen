#include "terrain.h"


Terrain::Terrain(QOpenGLFunctions_4_4_Core* functions): SceneObject()
{
    this->functions = functions;
    initVAO();
}

Terrain::~Terrain()
{

}

void Terrain::draw()
{
    functions->glPatchParameteri(GL_PATCH_VERTICES, 3);
    functions->glBindVertexArray(terrainVAO);
    functions->glDrawArrays(GL_PATCHES, 0, 6);
    functions->glBindVertexArray(0);
}

void Terrain::initVAO()
{

    float vertices[] = {
        // Triangle 1
        -1.0f , 0.0f ,  1.0f,
        -1.0f , 0.0f , -1.0f,
         1.0f , 0.0f ,  1.0f,
        // Triangle 2
        -1.0f , 0.0f , -1.0f,
         1.0f , 0.0f ,  1.0f,
         1.0f , 0.0f , -1.0f
    };

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
    functions->glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float)* 3, vertices, GL_STATIC_DRAW);

    functions->glEnableVertexAttribArray(0);
    functions->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);


    qDebug("Generate uvVBO");
    // Buffer for the uvs

    functions->glGenBuffers(1, &uvBuffer);
    functions->glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
    functions->glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float)* 2, uvs, GL_STATIC_DRAW);

    functions->glEnableVertexAttribArray(1);
    functions->glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);

}
