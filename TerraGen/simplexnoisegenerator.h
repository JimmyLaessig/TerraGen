#ifndef SIMPLEXNOISEGENERATOR_H
#define SIMPLEXNOISEGENERATOR_H
#include <QImage>

class SimplexNoiseGenerator
{
public:

    static double Octavs;
    static double Persistence;
    static double Scale;

    SimplexNoiseGenerator();

    static QImage* Generate(int width, int height);
};

#endif // SIMPLEXNOISEGENERATOR_H
