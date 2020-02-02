#pragma once
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <vector>
#include "glm/glm.hpp"

class IndexedGeometry
{
public:
    IndexedGeometry() = default;

    ~IndexedGeometry()
    {
        destroyVAO();
    }

    std::vector<glm::vec3> vertices;

    std::vector<glm::vec2> uvs;

    std::vector<unsigned int> indices;

    GLuint VAO;
    GLuint vertexBuffer;
    GLuint uvBuffer;
    GLuint indexBuffer;

    void destroyVAO();

    void createVAO();

    // Generates a new Grid
    static IndexedGeometry* Grid(unsigned int gridSize, unsigned int numSubdivisions);

    // Generates a new Grid
    static IndexedGeometry* FullscreenQuad();

    // Generates a new Grid
    static IndexedGeometry* Cube();
};
