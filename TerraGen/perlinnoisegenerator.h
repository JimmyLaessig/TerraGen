#ifndef PERLINNOISEGENERATOR_H
#define PERLINNOISEGENERATOR_H

#include <QOpenGLTexture>
#include <QOpenGLFramebufferObject>
#include <QOpenGLFunctions_4_4_Core>
#include <QOpenGLShaderProgram>
#include <QImage>
#include <random>
#include "shaders.h"

class PerlinNoiseGenerator
{
public:
    PerlinNoiseGenerator();
    static int seed;
    static QImage* Generate(QOpenGLFunctions_4_4_Core *functions, int width, int height);
private:

    static std::vector<int> p;

    static double noise(double x, double y, double z);
    static double fade(double t);
    static double lerp(double t, double a, double b);
    static double grad(int hash, double x, double y, double z);



};

#endif // PERLINNOISEGENERATOR_H
