#version 440 core

layout (location = 0) in vec3 position_VS;
layout (location = 1) in vec2 texcoords_VS;

uniform vec2 tileSize = vec2(10.0);
uniform vec2 tileIndex = vec2(0.0);
uniform vec2 numTiles = vec2(1.0);


out vec3 position_CS;
out vec2 heightmap_texcoords_CS;
out vec2 color_texcoords_CS;



vec2 calculateHeightmapTexcoords(vec3 position)
{
    // Calculate maximum dimensions of terrain
    vec2 gridSize = numTiles * tileSize;
    // Max Coordinates from [-0.5 * tileSize, 0.5 * tileSize]to [0, tileSize]

    vec2 texcoords = position.xy  / tileSize;
    // Map Positions to UV-Coordinates by dividing by the total number of vertices in each dimension


    // Scales the given texcoords (in domain [0,1]) to the dimensions of the grid at the given coordinates
    vec2 min = tileIndex / numTiles;
    vec2 max = (tileIndex +1) / numTiles;
    texcoords =  (max - min) * texcoords + min;
    texcoords.y = 1 - texcoords.y;
    return texcoords;
}

vec2 calculateColorTexcoords(vec3 position)
{
    vec2 texcoords = position.xy;
    // Map Positions to UV-Coordinates by dividing by the total number of vertices in each dimension
    return texcoords;
}

void main(void)
{
    position_CS             = position_VS;
    color_texcoords_CS      = calculateColorTexcoords(position_VS);
    heightmap_texcoords_CS  = calculateHeightmapTexcoords(position_VS);
}
