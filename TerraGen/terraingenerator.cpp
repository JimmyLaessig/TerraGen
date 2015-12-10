#include "terraingenerator.h"
#include <string>
#include <vector>
#include <cmath>

int TerrainGenerator::dimX = 2;
int TerrainGenerator::dimY = 2;
int TerrainGenerator::textureRepeat = 1;


Terrain* TerrainGenerator::Generate(QOpenGLFunctions_4_4_Core *functions)
{
    // Offset each triangle such that the whole Terrain will be centered to the origin
    float offsetX = -(float)dimX / 2.0f;
    float offsetZ = (float)dimY / 2.0f;

    // Number of vertices
    int numVertices = dimX * dimY * 2 * 3;

    // Create Vertex Array
    std::vector<glm::vec3> vertices = std::vector<glm::vec3>();
    for (int j = 0; j < dimY; j++)
    {
        for(int i = 0; i < dimX; i++)
        {
            // TODO : Create UVS
            // Face 1
            glm::vec3 vertex0;
            vertex0.x = i + offsetX;
            vertex0.y = 0;
            vertex0.z = -j + offsetZ;

            glm::vec3 vertex1;
            vertex1.x = i + offsetX;
            vertex1.y = 0;
            vertex1.z = -(j + 1) + offsetZ;

            glm::vec3 vertex2;
            vertex2.x = i + 1 + offsetX;
            vertex2.y = 0;
            vertex2.z = -j + offsetZ;

            vertices.push_back(vertex0);
            vertices.push_back(vertex1);
            vertices.push_back(vertex2);


            // Upper Face
            glm::vec3 vertex3;
            vertex3.x= i + offsetX;
            vertex3.y = 0;
            vertex3.z = -(j + 1) + offsetZ;

            glm::vec3 vertex4;
            vertex4.x = i + 1 + offsetX;
            vertex4.y = 0;
            vertex4.z = -j + offsetZ;

            glm::vec3 vertex5;
            vertex5.x = i + 1 + offsetX;
            vertex5.y = 0;
            vertex5.z = -(j + 1) + offsetZ;

            vertices.push_back(vertex3);
            vertices.push_back(vertex4);
            vertices.push_back(vertex5);
        }
    }


    Terrain* terrain = new Terrain(functions);
    terrain->setGeometry(&vertices[0].x, numVertices);
    terrain->createVAO();

    return terrain;
}
