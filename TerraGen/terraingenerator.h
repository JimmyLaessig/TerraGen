#ifndef TERRAINGENERATOR_H
#define TERRAINGENERATOR_H
#include "terrain.h"
#include "glm/vec3.hpp"
class TerrainGenerator
{
public:
    // Dimensions of the terrain
    static int dimX;
    static int dimY;
    // Number of times the textures are repeated
    static int textureRepeat;
    // Generates a new Terrain
    static Terrain* Generate(QOpenGLFunctions_4_4_Core* functions);

};

struct Face
{
    glm::vec3 v0;
    glm::vec3 v1;
    glm::vec3 v2;
};

#endif // TERRAINGENERATOR_H
