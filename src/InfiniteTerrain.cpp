#include "InfiniteTerrain.h"

#include <stdlib.h>
#include <glm/gtc/type_ptr.hpp>
#include <memory>
#include <array>
#include "Canvas.h"
#include <AABB.h>
#include<QElapsedTimer>
#include<iostream>
#include <QDebug>

//InfiniteTerrain::~Terrain()
//{
////    if(heightmapTexture)
////        heightmapTexture->destroy();
////    if(grassTexture)
////        grassTexture->destroy();
////    if(rockTexture)
////        rockTexture->destroy();
//}

void InfiniteTerrain::initializeGL()
{
//    this->grassTexture  = std::unique_ptr<QOpenGLTexture>(new QOpenGLTexture(QImage("Assets/textures/GrassGreenTexture0003.jpg")));
//    this->rockTexture   = std::unique_ptr<QOpenGLTexture>(new QOpenGLTexture(QImage("Assets/textures/rock.jpg")));
    grid                = std::unique_ptr<IndexedGeometry>(IndexedGeometry::Grid(tileSize, 10));
    grid->createVAO();
}



float calculateViewFrustumWidthAtFarPlane(const Camera& camera)
{
    auto fov    = glm::radians(camera.fov);
    auto fp     = camera.zFar;
    return tan(fov * 0.5f) * 2.0f * fp;
}


bool viewFrustumCulling(const Camera& camera, glm::vec2 cellPosition, float cellSize, float cellHeight)
{
    auto cp = camera.transform.getPosition();

    std::array<glm::vec3, 8> points =
    {
        cp + glm::vec3(cellPosition, 0.0f) + glm::vec3(0,        0,          0           ), // 0,0,0
        cp + glm::vec3(cellPosition, 0.0f) + glm::vec3(0,        0,          cellHeight  ), // 0,0,1
        cp + glm::vec3(cellPosition, 0.0f) + glm::vec3(0,        cellSize,   cellHeight  ), // 0,1,0
        cp + glm::vec3(cellPosition, 0.0f) + glm::vec3(0,        cellSize,   cellHeight  ), // 0,1,1
        cp + glm::vec3(cellPosition, 0.0f) + glm::vec3(cellSize, 0,          0           ), // 1,0,0
        cp + glm::vec3(cellPosition, 0.0f) + glm::vec3(cellSize, 0,          cellHeight  ), // 1,0,1
        cp + glm::vec3(cellPosition, 0.0f) + glm::vec3(cellSize, cellSize,   0           ), // 1,1,0
        cp + glm::vec3(cellPosition, 0.0f) + glm::vec3(cellSize, cellSize,   cellHeight  ) // 1,1,1
    };

    return false;
}

void collectCellPositionsRecursive(std::vector<glm::vec3>& positions, glm::vec2 cellPosition, float cellSize, float cellHeight, float minCellSize, const Camera& camera)
{
    // Stop the recursion of the cell is not visible
    if(viewFrustumCulling(camera, cellPosition, cellSize, cellHeight))
    {
        return;
    }

    // Stop the if we reached to leaves
    if(cellSize <= minCellSize)
    {
        positions.emplace_back(cellPosition.x, cellPosition.y, 0.0f);
        return;
    }

    collectCellPositionsRecursive(positions, cellPosition + glm::vec2(0, 0)                 , cellSize * 0.5f, cellHeight, minCellSize, camera);
    collectCellPositionsRecursive(positions, cellPosition + glm::vec2(cellSize, 0)          , cellSize * 0.5f, cellHeight, minCellSize, camera);
    collectCellPositionsRecursive(positions, cellPosition + glm::vec2(0, cellSize)          , cellSize * 0.5f, cellHeight, minCellSize, camera);
    collectCellPositionsRecursive(positions, cellPosition + glm::vec2(cellSize, cellSize)   , cellSize * 0.5f, cellHeight, minCellSize, camera);
}


std::vector<glm::vec3> InfiniteTerrain::computeVisibleTilePositions(const Camera& camera)
{
    QElapsedTimer timer;
    timer.start();
    //auto log_cellSize = logf(tileSize);

    // Calculate the theoretically max size of a tile to cover the entire screen.
    // The maxium cell size that spawns the entire view frustum is the maximum of the far plane and the view frustum width at the farplane
   // auto maxTileSize = std::max(camera.zFar, calculateViewFrustumWidthAtFarPlane(camera)) * 2.0f;

    // Find the next larger than maxTileSize power of two of the tileSize
   // auto exp = std::ceil(logf(maxTileSize) / log_cellSize);

    //auto cellSize_lod0 = powf(tileSize, exp);

    // Find the quadtree root next to the camera
    //auto cp = camera.transform.getPosition();

    auto proj = camera.getProjectionMatrix();
    auto view = camera.getViewMatrix();
    auto cameraPos2D = glm::vec2(camera.transform.getPosition());
    auto invViewProj = glm::inverse(proj * view);


    std::vector<glm::vec4> points = {
        invViewProj * glm::vec4(1, 1, 1, 1),
        invViewProj * glm::vec4(1, -1, 1, 1),
        invViewProj * glm::vec4(-1, 1, 1, 1),
        invViewProj * glm::vec4(-1, -1, 1, 1),

    };

    std::vector<glm::vec2> points2D;
    std::transform(std::begin(points), std::end(points),std::back_inserter(points2D), [&](const glm::vec4& point)
    {
       return cameraPos2D + glm::normalize(glm::vec2(point.x / point.w, point.y / point.w) - cameraPos2D) * drawDistance;
    });
    points2D.push_back(cameraPos2D);


    auto [minX, maxX] = std::minmax_element(std::begin(points2D), std::end(points2D), [](const auto& a, const auto& b){return a.x < b.x;});
    auto [minY, maxY] = std::minmax_element(std::begin(points2D), std::end(points2D), [](const auto& a, const auto& b){return a.y < b.y;});

    glm::vec2 p0(minX->x, minY->y);

    glm::vec2 gridStart(
        tileSize * floor(p0.x / tileSize),
        tileSize * floor(p0.y / tileSize));

    glm::vec2 p1(maxX->x, maxY->y);

    glm::vec2 gridEnd(
            tileSize * ceil(p1.x / tileSize),
            tileSize * ceil(p1.y / tileSize));


    std::vector<glm::vec3> positions;
    positions.reserve(size_t((gridEnd.x - gridStart.x) / tileSize * (gridEnd.y - gridStart.y) / tileSize));

    for(auto x = gridStart.x; x < gridEnd.x; x += tileSize)
    {
         for(auto y = gridStart.y; y < gridEnd.y; y += tileSize)
         {
             positions.push_back({x, y, 0});
         }
    }

    // Iterate the quad tree recursively starting by lod0 and collect the position of all leave cells that are within the viewFrustum
    //collectCellPositionsRecursive(positions, cellPosition_lod0, cellSize_lod0,maxHeight, tileSize, camera);
    return positions;
}


void InfiniteTerrain::renderTiles(const Camera& camera, const QOpenGLShaderProgram& shader)
{
    auto f = Canvas::OpenGLFunctions;
    f->glBindVertexArray(grid->VAO);
    //auto tilePositions =this->computeVisibleTilePositions(*Camera::Main);
    glm::mat4 worldMatrix(1);

    f->glUniformMatrix4fv(shader.uniformLocation("worldMatrix"), 1, GL_FALSE, glm::value_ptr(worldMatrix));
    f->glDrawElements(GL_TRIANGLES, (GLsizei)grid->indices.size(), GL_UNSIGNED_INT, nullptr);//, (GLsizei)tilePositions.size() );
    f->glBindVertexArray(0);
}

//void Terrain::drawTesselate()
//{
//    Canvas::OpenGLFunctions->glBindVertexArray(grid->VAO);
//    Canvas::OpenGLFunctions->glPatchParameteri(GL_PATCH_VERTICES, 3);
//    Canvas::OpenGLFunctions->glDrawElements(GL_PATCHES, (GLsizei)grid->indices.size(), GL_UNSIGNED_INT, nullptr);
//    Canvas::OpenGLFunctions->glBindVertexArray(0);
//}


//void Terrain::drawSimple()
//{
//    Canvas::OpenGLFunctions->glBindVertexArray(grid->VAO);
//    Canvas::OpenGLFunctions->glDrawElements(GL_TRIANGLES, (GLsizei)grid->indices.size(), GL_UNSIGNED_INT, nullptr);
//    Canvas::OpenGLFunctions->glBindVertexArray(0);
//}


//void Terrain::setHeightmapTexture(QImage *heightmapImage)
//{
//    // Destroy old Texture if available

//    heightmapTexture = std::unique_ptr<QOpenGLTexture>(new QOpenGLTexture(*heightmapImage));
//}

//void Terrain::setGridRepetitionX(unsigned int value)
//{
//    setGridRepetitions(value, gridRepetitionY);
//}

//void Terrain::setGridRepetitionY(unsigned int value)
//{
//    setGridRepetitions(gridRepetitionX, value);
//}

//unsigned int Terrain::getGridRepetitionX() const
//{
//    return gridRepetitionX;
//}

//unsigned int Terrain::getGridRepetitionY() const
//{
//    return gridRepetitionY;
//}

//void Terrain::setGridRepetitions(unsigned int x, unsigned int y)
//{
//    gridRepetitionX = x;
//    gridRepetitionY = y;

//    tiles.clear();
//    tileCoordinates.clear();

//    for(unsigned int i = 0; i < gridRepetitionX; i++)
//    {
//        for(unsigned int j = 0; j < gridRepetitionY; j++)
//        {
//            float translateX = gridSize * i;
//            float translateY = gridSize * j;

//            tiles.emplace_back();
//            tiles.back().setPosition({translateX, translateY, 0});
//            tileCoordinates.emplace_back(i, j);
//        }
//    }
//}


//void Terrain::setGrid(IndexedGeometry* grid)
//{
//    // Set new gird
//    this->grid = std::unique_ptr<IndexedGeometry>(grid);
//    // Create Grid on GPU
//    this->grid->createVAO();
//}


//void Terrain::destroyVAO()
//{
//    if(functions)
//    {
//        if (grid->indexBuffer)   functions->glDeleteBuffers(1, &grid.indexBuffer);
//        if (grid->vertexBuffer)  functions->glDeleteBuffers(1, &grid.vertexBuffer);
//        if (grid->uvBuffer)      functions->glDeleteBuffers(1, &grid.uvBuffer);
//        if (grid->terrainVAO)    functions->glDeleteVertexArrays(1, &grid.terrainVAO);
//    }
//}



