#ifndef RENDERER_H
#define RENDERER_H
#include <QOpenGLFunctions_4_4_Core>
#include <QOpenGLContext>
#include <shaders.h>
#include "terrain.h"
#include "camera.h"
#include "shadowmaptechnique.h"
#include "directionallight.h"

class Renderer
{
public:

    Renderer(QOpenGLContext* context, int width, int height);
    ~Renderer();

    void paintGL(Terrain* terrain);

    void resizeGL();


private:






};

#endif // RENDERER_H
