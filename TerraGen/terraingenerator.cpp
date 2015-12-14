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
    std::vector<float> vertices = std::vector<float>();
    std::vector<float> uvs = std::vector<float>();

    for (int j = 0; j <= dimY; j++)
    {
        for(int i = 0; i <= dimX; i++)
        {
            float x = i + offsetX;
            float y = 0;
            float z = -j + offsetZ;

            vertices.push_back(x);
            vertices.push_back(y);
            vertices.push_back(z);

            float uv_x = i  / (float)dimX;
            float uv_y = j  / (float)dimY ;

            uvs.push_back(uv_x);
            uvs.push_back(uv_y);

            int index = j * (dimX+1) + i;
            // qDebug("Vertex %d: (%f, %f, %f), uvs (%f,%f)", index, vertex.x, vertex.y, vertex.z, uv.x, uv.y);
        }
    }
    qDebug("Created Vertices");
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
    qDebug("Created Indices");
    Terrain* terrain = new Terrain(functions);

    Geometry geometry;
    geometry.numIndices = numIndices;
    geometry.numVertices = numVertices;

    geometry.indices = indices.data();
    geometry.vertices = vertices.data();
    geometry.uvs = uvs.data();

    terrain->setGeometry(geometry);
    terrain->createVAO();

    return terrain;
}
