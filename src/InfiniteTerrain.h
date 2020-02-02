#pragma once
#include "IndexedGeometry.h"
#include "Transform.h"
#include "Camera.h"

#include <QOpenGLTexture>
#include <QImage>
#include <QOpenGLShaderProgram>
#include <memory>
#include <vector>


class InfiniteTerrain
{
public:


    InfiniteTerrain() = default;


    virtual ~InfiniteTerrain() = default;


    void initializeGL();


    std::vector<glm::vec3> computeVisibleTilePositions(const Camera& camera);


    void renderTiles(const Camera& camera, const QOpenGLShaderProgram& shader);


   // std::vector<Transform> tiles;

   // std::vector<glm::vec2> tileCoordinates;

//    void drawTesselate();

//    void drawSimple();

//    void setHeightmapTexture(QImage* heightmapImage);

//    void setGrid(IndexedGeometry* grid);

//    unsigned int getGridRepetitionX() const;

//    unsigned int getGridRepetitionY() const;

//    void setGridRepetitionX(unsigned int value);

//    void setGridRepetitionY(unsigned int value);

//    void setGridRepetitions(unsigned int x, unsigned int y);

public:

//    unsigned int gridRepetitionX = 1;

//    unsigned int gridRepetitionY = 1;


    float drawDistance = 1000;

    unsigned int tileSize = 50;

    float maxHeight = 10.0f;

    std::unique_ptr<IndexedGeometry> grid;

//    std::unique_ptr<QOpenGLTexture> heightmapTexture;

//    std::unique_ptr<QOpenGLTexture> grassTexture;

//    std::unique_ptr<QOpenGLTexture> rockTexture;
};
