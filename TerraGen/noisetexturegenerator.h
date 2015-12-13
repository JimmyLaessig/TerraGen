#ifndef NOISETEXTUREGENERATOR_H
#define NOISETEXTUREGENERATOR_H

#include <QOpenGLTexture>
#include <QOpenGLFramebufferObject>
#include <QOpenGLFunctions_4_4_Core>
#include <QOpenGLShaderProgram>
#include <QImage>
#include "shaders.h"

class NoiseTextureGenerator
{
public:
    NoiseTextureGenerator();

    static QImage* Generate(QOpenGLFunctions_4_4_Core *functions, int width, int height);

};

#endif // NOISETEXTUREGENERATOR_H
