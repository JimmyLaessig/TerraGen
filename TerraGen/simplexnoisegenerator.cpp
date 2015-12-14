#include "simplexnoisegenerator.h"
#include <cmath>
#include "simplexnoise.h"

double SimplexNoiseGenerator::Octavs = 3;
double SimplexNoiseGenerator::Persistence = 0.5;
double SimplexNoiseGenerator::Scale = 0.01;


SimplexNoiseGenerator::SimplexNoiseGenerator(){}


QImage* SimplexNoiseGenerator::Generate(int width, int height)
{

    QImage* image = new QImage(width, height, QImage::Format_RGB888);
    for(int i = 0; i < width; i++)
    {
        for (int j = 0; j < height; j++)
        {
            double n = scaled_octave_noise_2d(Octavs, Persistence, Scale, 0, 1, i, j) * 255;

            image->setPixel(i, j, qRgb(n, n, n));
        }
    }

    return image;
}
