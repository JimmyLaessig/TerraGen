#pragma once
#include<QOpenGLTexture>
#include<QOpenGLFunctions_4_5_Core>

class SkyboxTechnique
{
public:

    SkyboxTechnique(QOpenGLFunctions_4_5_Core* functions);

    ~SkyboxTechnique();
    void draw();

private:

    QOpenGLFunctions_4_5_Core* functions;

    GLuint cubeVBO;
    GLuint cubeVAO;

    QOpenGLTexture* cubeMapTexture;
};
