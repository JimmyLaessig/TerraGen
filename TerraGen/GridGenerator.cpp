#include "gridgenerator.h"
#include <string>
#include <vector>
#include <cmath>

int GridGenerator::gridSize = 10;

Terrain* GridGenerator::Generate(QOpenGLFunctions_4_4_Core *functions)
{

    // Number of vertices
    int numVertices = gridSize * gridSize * 2 * 3;

    // Create Vertex Array
    std::vector<float> vertices = std::vector<float>();
    std::vector<float> uvs = std::vector<float>();

    for (int j = 0; j <= gridSize; j++)
    {
        for(int i = 0; i <= gridSize; i++)
        {
            float x = i;
            float y = 0;
            float z = -j;

            vertices.push_back(x);
            vertices.push_back(y);
            vertices.push_back(z);

            float uv_x = i  / (float)gridSize;
            float uv_y = j  / (float)gridSize ;

            uvs.push_back(uv_x);
            uvs.push_back(uv_y);


            int index = j * (gridSize+1) + i;
            // qDebug("Vertex %d: (%f, %f, %f), uvs (%f,%f)", index, x, y, z, uv_x, uv_y);
        }
    }
    qDebug("Created Vertices");
    // Create Index Array
    int numFaces = gridSize * gridSize * 2;
    int numIndices = numFaces * 3;
    std::vector<unsigned int> indices = std::vector<unsigned int>();

    int numX = gridSize + 1;
    for(int y = 0; y < gridSize; y++)
    {
        for(int x = 0; x< gridSize; x++ )
        {
            // Lower Face
            int index0 = y * numX + x;

            int index1 = y * numX + (x + 1);
            int index2 = (y + 1) * numX + x;


            indices.push_back(index0);
            indices.push_back(index1);
            indices.push_back(index2);

            //  qDebug("Face: (%d, %d, %d)",index0, index1, index2 );

            // Upper Face
            int index3 = (y + 1) * numX + x;

            int index4 = y * numX + (x + 1);
            int index5 = (y + 1) * numX + (x + 1);

            indices.push_back(index3);
            indices.push_back(index4);
            indices.push_back(index5);

            //  qDebug("Face: (%d, %d, %d)",index3, index4, index5 );
        }
    }
    qDebug("Created Indices");
    Terrain* terrain = new Terrain(functions);

    Grid grid;
    grid.gridSize = gridSize;
    grid.numIndices = numIndices;
    grid.numVertices = numVertices;

    grid.indices = indices.data();
    grid.vertices = vertices.data();
    grid.uvs = uvs.data();

    terrain->setGrid(grid);

    return terrain;
}
