#include "renderer.h"
#include "glm/gtc/type_ptr.hpp"

Renderer::Renderer(QOpenGLContext* context, int width, int height)
{
    this->context = context;
    this->width = width;
    this->height = height;



}

Renderer::~Renderer()
{
    delete shadowMaptechnique;
    delete light;
}

void Renderer::paintGL(Terrain* terrain)
{

}




void Renderer::resizeGL()
{
    // TODO RESIZE FRAMEBUFFERS HERE
}

