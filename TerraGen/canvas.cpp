#include "canvas.h"
#include "QMatrix4x4"
#include "shaders.h"
#include <QKeyEvent>
#include "glm/gtc/type_ptr.hpp"
#include <iostream>
#include "terraingenerator.h"


Canvas::Canvas(QWidget* parent) : QOpenGLWidget(parent)
{
    this->window = static_cast<Window*>(parent);
    mouseLeftDown = false;
    mouseRightDown = false;
}

Canvas::~Canvas()
{
    delete terrain;
    delete camera;
}

void Canvas::initializeGL()
{
    initializeOpenGLFunctions();

    Shaders::InitializeShaders();

    noiseImage = new QImage(512, 512, QImage::Format_RGB16);
    noiseImage->fill(qRgb(255, 255, 255));
    window->setNoiseImage(noiseImage);

    renderer = new Renderer(this, this->size().width(), this->size().height());
    terrain = nullptr;
    camera = new Camera();
    float ratio = (float)this->size().width()/ (float)this->size().height();
    camera->setProjectionMatrix(70.0f, ratio, 0.1f, 1000.0f);
    camera->translate(glm::vec3(0, 2, 0));
    camera->rotate(glm::vec3(1,0,0), 90.0f);

    renderer->camera = camera;
}

void Canvas::paintGL()
{
    // Generate Noise Texture
    if(generateNoise)
    {
        // Delete old NoiseImage
        if(noiseImage != nullptr)
            delete noiseImage;
        // Generate new NoiseImage
        noiseImage = PerlinNoiseGenerator::Generate(this, 512, 512);

        // Create Noise Texture if Terrain is available
        if(terrain!= nullptr)
            terrain->setNoiseTexture(noiseImage);

        // Update UI
        window->setNoiseImage(noiseImage);

        generateNoise = false;
    }

    // Generate Terrain
    if(generateTerrain)
    {
        // Delete old Terrain
        if(terrain !=nullptr)
            delete terrain;

        // Generate new Terrain
        terrain = TerrainGenerator::Generate(this);
        // Set the Noise Texture to the current NoiseImage
        terrain->setNoiseTexture(noiseImage);

        generateTerrain = false;
    }


    // Clear the Canvas
    glViewport(0,0,size().width(), size().height());
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

    // return if no terrain exists
    if(terrain == nullptr)
        return;

    // Render terrain
    renderer->paintGL(terrain);

}

void Canvas::resizeGL()
{
}

void Canvas::mouseMoveEvent(QMouseEvent * event)
{
    // Rotation
    if(mouseLeftDown)
    {
        return;
    }

    // Translation
    if(mouseRightDown)
    {
        return;
    }
}

void Canvas::wheelEvent(QWheelEvent * event)
{
    float factor = 0.01f * event->angleDelta().y() * 0.1f;
    camera->translate( camera->forward()* factor);
    update();
}

void Canvas::mousePressEvent(QMouseEvent * event)
{
    if(event->button() == Qt::RightButton)
    {
        mouseRightDown = true;
    }
    if(event->button() == Qt::LeftButton)
    {
        mouseLeftDown = true;
    }
}
void Canvas::mouseReleaseEvent(QMouseEvent * event)
{
    if(event->button() == Qt::RightButton)
    {
        mouseRightDown = false;
    }
    if(event->button() == Qt::LeftButton)
    {
        mouseLeftDown = false;
    }
}


void Canvas::keyPressEvent(QKeyEvent* event)
{

}

void Canvas::dimXValueChanged(int value)
{
    TerrainGenerator::dimX = value;
}

void Canvas::dimYValueChanged(int value)
{
    TerrainGenerator::dimY = value;
}

void Canvas::generateTerrainButtonClicked()
{
    generateTerrain = true;
    update();
}

void Canvas::openNoiseTextureButtonClicked()
{
    update();
}

void Canvas::noiseTypeChanged(QString type)
{
    update();
}

void Canvas::generateNoiseTextureButtonClicked()
{
    generateNoise = true;
    update();
}

void Canvas::heightValueChanged(double value)
{
    terrain->height = value;
    update();
}

void Canvas::wireframeEnabled(bool enabled)
{
    renderer->wireframeEnabled = enabled;
    update();
}

void Canvas::shadingEnabled(bool enabled)
{
    renderer->shadingEnabled = enabled;
    update();
}

void Canvas::dynamicLoDEnabled(bool enabled)
{
    qDebug("bla");
    renderer->dynamicLoDEnabled = enabled;
    update();
}

void Canvas::textureRepeatValueChanged(double value)
{
    terrain->texcoordScale = (float) value;
    update();
}

void Canvas::dynamicTexturingEnabled(bool enabled)
{
    renderer->dynamicTexturingEnabled = enabled;
    update();
}

void Canvas::normalMappingEnabled(bool enabled)
{
    renderer->normalMappingEnabled = enabled;
    update();
}

void Canvas::shadowsEnabled(bool enabled)
{
    renderer->shadowsEnabled = enabled;
    update();
}

void Canvas::distanceFogEnabled(bool enabled)
{
    renderer->distanceFogEnabled = enabled;
    update();
}



