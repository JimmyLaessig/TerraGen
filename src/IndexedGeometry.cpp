#include "IndexedGeometry.h"
#include <string>
#include <vector>
#include <cmath>
#include "glm/glm.hpp"
#include "Canvas.h"

IndexedGeometry* IndexedGeometry::Grid(unsigned int gridSize, unsigned int numSubdivisions)
{

    // Create Vertex Array
    IndexedGeometry* grid = new IndexedGeometry();
    grid->vertices.clear();
    grid->indices.clear();
   // grid->gridSize   = gridSize;

    for (unsigned int j = 0; j <= numSubdivisions; j++)
    {
        for(unsigned int i = 0; i <= numSubdivisions; i++)
        {
            float x = float(i) / numSubdivisions * gridSize;
            float y = float(j) / numSubdivisions * gridSize;

            grid->vertices.emplace_back(x, y, 0);

            float uv_x = i  / float(gridSize);
            float uv_y = j  / float(gridSize);

            grid->uvs.emplace_back(uv_x, uv_y);
        }
    }
    qDebug("Created Vertices");
    auto numX = numSubdivisions + 1;
    for(unsigned int y = 0; y < numSubdivisions; y++)
    {
        for(unsigned int x = 0; x < numSubdivisions; x++ )
        {
            auto i0 = y * numX + x;
            auto i1 = y * numX + x + 1;
            auto i2 = (y + 1) * numX + x;
            auto i3 = (y + 1) * numX + x + 1;

            // Lower Face
            grid->indices.push_back(i0);
            grid->indices.push_back(i1);
            grid->indices.push_back(i2);

            // Upper Face
            grid->indices.push_back(i2);
            grid->indices.push_back(i1);
            grid->indices.push_back(i3);
        }
    }

    qDebug("Created Indices");

    return grid;
}


IndexedGeometry* IndexedGeometry::FullscreenQuad()
{
    IndexedGeometry* quad = new IndexedGeometry();

    quad->vertices =
    {
        {-1, -1, 0},
        { 1, -1, 0},
        {-1,  1, 0},
        { 1,  1, 0}
    };
    quad->indices =
    {
        0, 1, 2,
        2, 1, 3
    };
    quad->uvs =
    {
        {0, 0},
        {1, 0},
        {0, 1},
        {1, 1}
    };
    return quad;
}


IndexedGeometry* IndexedGeometry::Cube()
{
    IndexedGeometry* cube = new IndexedGeometry();

    cube->vertices =
    {
        // front
        {-1.0, -1.0,  1.0},
        { 1.0, -1.0,  1.0},
        { 1.0,  1.0,  1.0},
        {-1.0,  1.0,  1.0},
        // back
        {-1.0, -1.0, -1.0},
        { 1.0, -1.0, -1.0},
        { 1.0,  1.0, -1.0},
        {-1.0,  1.0, -1.0}
    };
    cube->indices =
    {
        // front
        0, 1, 2,
        2, 3, 0,
        // right
        1, 5, 6,
        6, 2, 1,
        // back
        7, 6, 5,
        5, 4, 7,
        // left
        4, 0, 3,
        3, 7, 4,
        // bottom
        4, 5, 1,
        1, 0, 4,
        // top
        3, 2, 6,
        6, 7, 3
    };

    return cube;
}



void IndexedGeometry::destroyVAO()
{
    auto functions = Canvas::OpenGLFunctions;
    functions->glDeleteBuffers(1, &this->uvBuffer);
    functions->glDeleteBuffers(1, &this->vertexBuffer);
    functions->glDeleteBuffers(1, &this->indexBuffer);
    functions->glDeleteVertexArrays(1, &this->VAO);
}


void IndexedGeometry::createVAO()
{
    destroyVAO();
//   if( this->VAO.isCreated())
//       this->VAO.destroy();
//   if(this->indexBuffer.isCreated())
//       this->indexBuffer.destroy();
//   if(this->vertexBuffer.isCreated())
//       this->indexBuffer.destroy();

//    this->VAO.create();
//    this->VAO.bind();

//    this->indexBuffer = QOpenGLBuffer(QOpenGLBuffer::IndexBuffer);
//    this->indexBuffer.setUsagePattern(QOpenGLBuffer::UsagePattern::StaticDraw);
//    this->indexBuffer.allocate(this->indices.data(), this->indices.size() * sizeof(unsigned int));

//    this->vertexBuffer = QOpenGLBuffer(QOpenGLBuffer::Type::VertexBuffer);
//    this->vertexBuffer.setUsagePattern(QOpenGLBuffer::UsagePattern::StaticDraw);
//    this->vertexBuffer.allocate(this->vertices.data(), this->vertices.size() * sizeof(glm::vec3));

//    this->uvBuffer = QOpenGLBuffer(QOpenGLBuffer::Type::VertexBuffer);
//    this->uvBuffer.setUsagePattern(QOpenGLBuffer::UsagePattern::StaticDraw);
//    this->uvBuffer.allocate(this->uvs.data(), this->uvs.size() * sizeof(glm::vec2));

   // this->vertexBuffer.bind();

//
//

//    this->uvBuffer.bind();


    auto functions = Canvas::OpenGLFunctions;

   // Bind VAO
    functions->glGenVertexArrays(1, &VAO);
    functions->glBindVertexArray(VAO);

    // Buffer for the indices
    functions->glGenBuffers(1, &indexBuffer);
    functions->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
    functions->glBufferData(GL_ELEMENT_ARRAY_BUFFER,  GLsizei(indices.size() * sizeof(unsigned int)), &indices[0], GL_STATIC_DRAW);
    //qDebug("Created IndexBuffer");

    // Buffer for the vertices
    functions->glGenBuffers(1, &vertexBuffer);
    functions->glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    functions->glBufferData(GL_ARRAY_BUFFER, GLsizei(vertices.size() * sizeof(glm::vec3)), glm::value_ptr(vertices[0]), GL_STATIC_DRAW);

    functions->glEnableVertexAttribArray(0);
    functions->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
    // qDebug("Created vertexbuffer");

    // Buffer for the uvs
    if(uvs.size())
    {
        functions->glGenBuffers(1, &uvBuffer);
        functions->glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
        functions->glBufferData(GL_ARRAY_BUFFER, GLsizei(uvs.size() * sizeof(glm::vec2)), glm::value_ptr(uvs[0]), GL_STATIC_DRAW);

        functions->glEnableVertexAttribArray(1);
        functions->glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
    }
  //  qDebug("Created uvBuffer");

    // Release VAO

    functions->glBindVertexArray(0);
}
