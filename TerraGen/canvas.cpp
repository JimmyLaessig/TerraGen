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
}

Canvas::~Canvas()
{
    delete terrain;
    delete camera;
    Shaders::DeleteAll();
}

void Canvas::initializeGL()
{
    initializeOpenGLFunctions();

    lastMousePos = QCursor::pos();

    Shaders::InitializeShaders();

    noiseImage = new QImage(512, 512, QImage::Format_RGB16);
    noiseImage->fill(qRgb(255, 255, 255));
    window->setNoiseImage(noiseImage);

    terrain = GridGenerator::Generate(this);
    terrain->setHeightmapTexture(noiseImage);

    camera = new Camera();
    float ratio = (float)size().width( )/ (float)size().height();
    camera->setProjectionMatrix(70.0f, ratio, 0.1f, 1000.0f);
    camera->setPosition(glm::vec3(0, 5, 0));
    camera->rotate(glm::vec3(0, 1, 0), 45);
    camera->rotate(glm::vec3(1, 0, 0), 45);

    renderer = new Renderer(this, this->size().width(), this->size().height());
    renderer->camera = camera;

    cameraController = new CameraController();
    cameraController->setCamera(camera);

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
        noiseImage = HeightmapGenerator::Generate(512, 512);

        // Create Noise Texture if Terrain is available
        terrain->setHeightmapTexture(noiseImage);

        // update UI
        window->setNoiseImage(noiseImage);

        generateNoise = false;
    }
    QTime time = QTime::currentTime();

    // Clear the Canvas
    glViewport(0,0,size().width(), size().height());
    glDepthMask(GL_TRUE);
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);


    // Render terrain
    renderer->paintGL(terrain);

    // Display last frametime
    double ms = (double)time.msecsTo(QTime::currentTime());
    window->setFPSLabel(1000.0 / ms);
    // qDebug("Camera Forward: %f, %f, %f", camera->forward().x, camera->forward().y, camera->forward().z);
}

void Canvas::resizeGL()
{
}

void Canvas::mouseMoveEvent(QMouseEvent * event)
{
    if(event->buttons() == Qt::RightButton)
    {
        QPoint diff = QCursor::pos() - lastMousePos;
        camera->rotate(glm::vec3(0,1,0), (float)diff.x() * 0.09);
        camera->rotate(glm::vec3(1,0,0), (float)diff.y() * 0.09);
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

void Canvas::keyPressEvent(QKeyEvent* event)
{
    cameraController->keyPressed(event->key());
    update();
}

void Canvas::keyReleaseEvent(QKeyEvent *event)
{
    if(event->isAutoRepeat())
        return;

    cameraController->keyReleased(event->key());

    if(event->key() == Qt::Key_F1)
    {
        Shaders::DeleteAll();
        Shaders::InitializeShaders();
    }
    update();

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

void Canvas::noiseFrequency1Changed(double value)
{
    HeightmapGenerator::Frequency1 = value;
}

void Canvas::noiseFrequency2Changed(double value)
{
    HeightmapGenerator::Frequency2 = value;
}

void Canvas::noisePersistenceChanged(double value)
{
    HeightmapGenerator::Persistence = value;
}

void Canvas::noiseBiasChanged(double value)
{
    HeightmapGenerator::Bias = value;
}

void Canvas::noiseScaleChanged(double value)
{
    HeightmapGenerator::Scale = value;
}


