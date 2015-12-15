#ifndef GRIDGENERATOR_H
#define GRIDGENERATOR_H

#include "gridgenerator.h"
#include "terrain.h"
#include "glm/vec3.hpp"

class GridGenerator
{
public:
    // Dimensions of the terrain
    static int gridSize;

    // Number of times the textures are repeated
    static int textureRepeat;
    // Generates a new Terrain
    static Terrain* Generate(QOpenGLFunctions_4_4_Core* functions);

};

#endif // GRIDGENERATOR_H
