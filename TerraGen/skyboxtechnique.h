#ifndef SKYBOXTECHNIQUE_H
#define SKYBOXTECHNIQUE_H
#include <QOpenGLTexture>
#include<QOpenGLFunctions_4_4_Core>

class SkyboxTechnique
{
public:

    SkyboxTechnique(QOpenGLFunctions_4_4_Core* functions);

    ~SkyboxTechnique();
    void draw();

private:

    QOpenGLFunctions_4_4_Core* functions;

    GLuint cubeVBO;
    GLuint cubeVAO;

    QOpenGLTexture* cubeMapTexture;
};

#endif // SKYBOXTECHNIQUE_H
