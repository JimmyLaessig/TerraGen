#include "heightmapgenerator.h"

#include "noise/noise.h"
#include "noiseutils.h"


using namespace noise;

HeightmapGenerator::HeightmapGenerator(){}

double HeightmapGenerator::Frequency1 = 2.0;
double HeightmapGenerator::Scale = 0.125;
double HeightmapGenerator::Bias = -0.75;

double HeightmapGenerator::Frequency2 = 0.5;
double HeightmapGenerator::Persistence = 0.25;

QImage* HeightmapGenerator::Generate(int width, int height)
{
    module::RidgedMulti mountainTerrain;
    module::Billow baseFlatTerrain;
    baseFlatTerrain.SetFrequency(Frequency1);

    module::ScaleBias flatTerrain;
    flatTerrain.SetSourceModule(0, baseFlatTerrain);
    flatTerrain.SetScale(Scale);
    flatTerrain.SetBias(Bias);

    module::Perlin terrainType;
    terrainType.SetFrequency(Frequency2);
    terrainType.SetPersistence(Persistence);

    module::Select finalTerrain;
    finalTerrain.SetSourceModule(0, flatTerrain);
    finalTerrain.SetSourceModule(1, mountainTerrain);
    finalTerrain.SetControlModule(terrainType);
    finalTerrain.SetBounds(0.0, 1000.0);
    finalTerrain.SetEdgeFalloff(0.125);

    utils::NoiseMap heightMap;
    utils::NoiseMapBuilderPlane heightMapBuilder;
    heightMapBuilder.SetSourceModule(finalTerrain);
    heightMapBuilder.SetDestNoiseMap(heightMap);
    heightMapBuilder.SetDestSize(width, height);
    heightMapBuilder.SetBounds(6.0, 10.0, 1.0, 5.0);
    heightMapBuilder.Build();

    utils::RendererImage renderer;
    utils::Image image;
    renderer.SetSourceNoiseMap (heightMap);
    renderer.SetDestImage (image);
    renderer.Render ();

    QImage* heightMapImage = new QImage(width, height, QImage::Format_RGB888);

    for(int i = 0; i < width;i++)
    {
        for (int j = 0; j < height; j++)
        {
            auto c =  image.GetValue(i,j);
            heightMapImage->setPixel(i, j, qRgb(c.red, c.green, c.blue));
        }
    }

//    QImage* normalMapImage = new QImage(width, height, QImage::Format_RGB888);

//    utils::RendererNormalMap normalMapRenderer;
//    normalMapRenderer.SetSourceNoiseMap(heightMap);
//    normalMapRenderer.SetDestImage(image);
//    normalMapRenderer.SetBumpHeight(10.0);
//    normalMapRenderer.Render();


//    for(int i = 0; i < width;i++)
//    {
//        for (int j = 0; j < height; j++)
//        {
//            auto c =  image.GetValue(i,j);
//            normalMapImage->setPixel(i, j, qRgb(c.red, c.blue, c.green));
//        }
//    }

    return heightMapImage;
}
