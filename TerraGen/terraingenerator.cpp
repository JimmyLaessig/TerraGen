#include "terraingenerator.h"
#include <string>
#include <vector>
#include <cmath>

int TerrainGenerator::dimX = 10;
int TerrainGenerator::dimY = 10;

Terrain* TerrainGenerator::Generate(QOpenGLFunctions_4_4_Core *functions)
{
    // Offset each triangle such that the whole Terrain will be centered to the origin
    float offsetX = -(float)dimX / 2.0f;
    float offsetZ = (float)dimY / 2.0f;

    // Number of vertices
    int numVertices = dimX * dimY * 2 * 3;

    // Create Vertex Array
    std::vector<glm::vec3> vertices = std::vector<glm::vec3>();
    std::vector<glm::vec2> uvs = std::vector<glm::vec2>();
    for (int j = 0; j <= dimY; j++)
    {
        for(int i = 0; i <= dimX; i++)
        {
            glm::vec3 vertex;
            vertex.x = i + offsetX;
            vertex.y = 0;
            vertex.z = -j + offsetZ;
            vertices.push_back(vertex);

            glm::vec2 uv;
            uv.x = i  / (float)dimX;
            uv.y = j  / (float)dimY ;
            uvs.push_back(uv);

            int index = j * (dimX+1) + i;
            // qDebug("Vertex %d: (%f, %f, %f), uvs (%f,%f)", index, vertex.x, vertex.y, vertex.z, uv.x, uv.y);
        }
    }

    // Create Index Array
    int numFaces = dimX * dimY * 2;
    int numIndices = numFaces * 3;
    std::vector<unsigned int> indices = std::vector<unsigned int>();

    int numX = dimX+1;
    for(int j = 0; j < dimY; j++)
    {
        for(int i = 0; i< dimX; i++ )
        {
            // Lower Face
            int index0 = j * numX + i;
            int index1 = (j + 1) * numX + i;
            int index2 = j * numX + (i + 1);

            indices.push_back(index0);
            indices.push_back(index1);
            indices.push_back(index2);

            //  qDebug("Face: (%d, %d, %d)",index0, index1, index2 );

            // Upper Face
            int index3 = (j + 1) * numX + i;
            int index4 = j * numX + (i + 1);
            int index5 = (j + 1) * numX + (i + 1);

            indices.push_back(index3);
            indices.push_back(index4);
            indices.push_back(index5);

            //  qDebug("Face: (%d, %d, %d)",index3, index4, index5 );
        }
    }

    Terrain* terrain = new Terrain(functions);

    Geometry geometry;
    geometry.numIndices = numIndices;
    geometry.numVertices = numVertices;

    geometry.indices = &indices[0];
    geometry.vertices = &vertices[0].x;
    geometry.uvs = &uvs[0].x;

    terrain->setGeometry(geometry);

    terrain->createVAO();

    return terrain;
}
