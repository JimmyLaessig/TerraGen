#ifndef SHADOWMAPTECHNIQUE_H
#define SHADOWMAPTECHNIQUE_H
#include "QOpenGLFunctions_4_4_Core"
#include "terrain.h"
#include "directionallight.h"
class ShadowMapTechnique
{
public:
    ShadowMapTechnique(QOpenGLFunctions_4_4_Core* functions,int width, int height);

    void drawShadowMap(Terrain* terrain);

    void bindShadowTexture(int unit);
    DirectionalLight* light;

private:
    QOpenGLFunctions_4_4_Core* functions;

    GLuint fbo;
    GLuint depthTexture;

    int width;
    int height;
};

#endif // SHADOWMAPTECHNIQUE_H
