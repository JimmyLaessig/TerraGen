#include "canvas.h"
#include <QTime>
#include "shaders.h"
#include <QKeyEvent>
#include "glm/gtc/type_ptr.hpp"
#include <iostream>
#include "gridgenerator.h"



Canvas::Canvas(QWidget* parent) : QOpenGLWidget(parent)
{
    this->window = static_cast<Window*>(parent);
    mouseLeftDown = false;
    mouseRightDown = false;
    mouseSensitivity = 0.09f;
}

Canvas::~Canvas()
{
    delete terrain;
    delete camera;
}

void Canvas::initializeGL()
{
    initializeOpenGLFunctions();

    lastMousePos = QCursor::pos();

    Shaders::InitializeShaders();

    noiseImage = new QImage(512, 512, QImage::Format_RGB16);
    noiseImage->fill(qRgb(255, 255, 255));
    window->setNoiseImage(noiseImage);

    renderer = new Renderer(this, this->size().width(), this->size().height());

    terrain = GridGenerator::Generate(this);
    terrain->setHeightmapTexture(noiseImage);
    camera = new Camera();
    float ratio = (float)this->size().width()/ (float)this->size().height();
    camera->setProjectionMatrix(70.0f, ratio, 0.1f, 1000.0f);
    camera->translate(glm::vec3(0, 2, 0));
    camera->rotate(glm::vec3(1,0,0), 45.0f);

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
        noiseImage = SimplexNoiseGenerator::Generate(512, 512);

        // Create Noise Texture if Terrain is available
        terrain->setHeightmapTexture(noiseImage);

        // update UI
        window->setNoiseImage(noiseImage);

        generateNoise = false;
    }
    QTime time = QTime::currentTime();

    // Clear the Canvas
    glViewport(0,0,size().width(), size().height());
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);


    // Render terrain
    renderer->paintGL(terrain);

    // Display last frametime
    double ms = (double)time.msecsTo(QTime::currentTime());
    window->setFPSLabel(1000.0 / ms);

}

void Canvas::resizeGL()
{
}

void Canvas::mouseMoveEvent(QMouseEvent * event)
{
    if(event->buttons() == Qt::RightButton)
    {
        QPoint diff = QCursor::pos() - lastMousePos;
        camera->rotate(glm::vec3(0,1,0), (float)diff.x() * mouseSensitivity);
        camera->rotate(glm::vec3(1,0,0), (float)diff.y() * mouseSensitivity);
        update();
    }
    lastMousePos = QCursor::pos();

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

    if(event->key() == Qt::Key_W)
    {
        qDebug("forward");
        camera->translate(glm::vec3(0, 0, -0.01));
    }
    if(event->key() == Qt::Key_S)
    {
        qDebug("backwards");
        camera->translate(glm::vec3(0, 0, 0.01));
    }

    // update();
}




void Canvas::gridRepetitionXChanged(int value)
{
    terrain->setGridRepetitionX(value);
    update();
}

void Canvas::gridRepetitionYChanged(int value)
{
    terrain->setGridRepetitionY(value);
    update();
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
    terrain->maxHeight = value;
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

void Canvas::noiseOctavsChanged(double value)
{
    SimplexNoiseGenerator::Octavs = value;
}

void Canvas::noisePersistenceChanged(double value)
{
    SimplexNoiseGenerator::Persistence = value;
}

void Canvas::noiseScaleChanged(double value)
{
    SimplexNoiseGenerator::Scale = value;
}



