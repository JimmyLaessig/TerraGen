#ifndef HEIGHTMAPGENERATOR_H
#define HEIGHTMAPGENERATOR_H
#include <QImage>

class HeightmapGenerator
{
public:

    static double Octavs;
    static double Persistence;
    static double Scale;

    HeightmapGenerator();

    static QImage* Generate(int width, int height);
};

#endif // SIMPLEXNOISEGENERATOR_H
