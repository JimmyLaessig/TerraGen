#ifndef RENDERER_H
#define RENDERER_H
#include <QOpenGLFunctions_4_4_Core>
#include <shaders.h>
#include "terrain.h"
#include "camera.h"
#include "directionallight.h"

class Renderer
{
public:

    bool wireframeEnabled = true;
    bool shadingEnabled = true;
    bool dynamicLoDEnabled = true;
    bool dynamicTexturingEnabled = true;
    bool normalMappingEnabled = true;
    bool shadowsEnabled = true;
    bool distanceFogEnabled = true;


    Renderer(QOpenGLFunctions_4_4_Core* functions, int width, int height);
    ~Renderer();

    void paintGL(Terrain* terrain);

    void resizeGL();

    Camera* camera;

    QOpenGLFunctions_4_4_Core* functions;

private:

    DirectionalLight* light;

    int width;
    int height;

    void drawSimple(Terrain* terrain);
    void drawTesselate(Terrain* terrain);
};

#endif // RENDERER_H
