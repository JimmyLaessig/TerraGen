#ifndef HEIGHTMAPGENERATOR_H
#define HEIGHTMAPGENERATOR_H
#include <QImage>

class HeightmapGenerator
{
public:
    static double Frequency1;
    static double Frequency2;
    static double Bias;
    static double Persistence;
    static double Scale;

    HeightmapGenerator();

    static QImage* Generate(int width, int height);
};

#endif // SIMPLEXNOISEGENERATOR_H
