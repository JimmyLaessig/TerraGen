#include "perlinnoisegenerator.h"
#include <cmath>
#include <vector>

int PerlinNoiseGenerator::seed = 42;
std::vector<int> PerlinNoiseGenerator::p;

PerlinNoiseGenerator::PerlinNoiseGenerator()
{

}

QImage* PerlinNoiseGenerator::Generate(QOpenGLFunctions_4_4_Core *functions, int width, int height)
{

    // Fill p with values from 0 to 255
    p.resize(256);

    // Fill p with values from 0 to 255
    std::iota(p.begin(), p.end(), 0);

    // Initialize a random engine with seed
    std::default_random_engine random(seed);
    // Suffle  using the above random engine
    std::shuffle(p.begin(), p.end(), random);
    // Duplicate the permutation vector
    p.insert(p.end(), p.begin(), p.end());

    QImage* image = new QImage(width, height, QImage::Format_RGB888);

    for(int j = 0; j < height; j++)
    {
        for(int i = 0; i < width; i++)
        {
            double x = (double)i / (double)height;
            double y = (double)j / (double)height;

            double n = 20 * noise(10 * x, 10 * y, 0.8);
          //  n = n - floor(n);
            image->setPixel(i, j, qRgb(255 * n, 255 * n, 255 * n));
        }
    }
    return image;
}

double PerlinNoiseGenerator::noise(double x, double y, double z)
{
    // Find the unit cube that contains the point
    int X = (int) floor(x) & 255;
    int Y = (int) floor(y) & 255;
    int Z = (int) floor(z) & 255;

    // Find relative x, y,z of point in cube
    x -= floor(x);
    y -= floor(y);
    z -= floor(z);

    // Compute fade curves for each of x, y, z
    double u = fade(x);
    double v = fade(y);
    double w = fade(z);

    // Hash coordinates of the 8 cube corners
    int A = p[X] + Y;
    int AA = p[A] + Z;
    int AB = p[A + 1] + Z;
    int B = p[X + 1] + Y;
    int BA = p[B] + Z;
    int BB = p[B + 1] + Z;

    // Add blended results from 8 corners of cube
    double res = lerp(w, lerp(v, lerp(u, grad(p[AA], x, y, z), grad(p[BA], x-1, y, z)), lerp(u, grad(p[AB], x, y-1, z), grad(p[BB], x-1, y-1, z))),	lerp(v, lerp(u, grad(p[AA+1], x, y, z-1), grad(p[BA+1], x-1, y, z-1)), lerp(u, grad(p[AB+1], x, y-1, z-1),	grad(p[BB+1], x-1, y-1, z-1))));
    return (res + 1.0)/2.0;
}

double PerlinNoiseGenerator::fade(double t)
{
    return t * t * t * (t * (t * 6 - 15) + 10);
}
double PerlinNoiseGenerator::lerp(double t, double a, double b)
{
    return a + t * (b - a);
}

double PerlinNoiseGenerator::grad(int hash, double x, double y, double z)
{
    int h = hash & 15;
    // Convert lower 4 bits of hash inot 12 gradient directions
    double u = h < 8 ? x : y,
            v = h < 4 ? y : h == 12 || h == 14 ? x : z;
    return ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v);
}
