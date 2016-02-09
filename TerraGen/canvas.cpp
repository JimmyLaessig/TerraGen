#include "canvas.h"
#include <QTime>
#include "shaders.h"
#include <QKeyEvent>
#include "glm/gtc/type_ptr.hpp"
#include <iostream>
#include "gridgenerator.h"
#include <algorithm>
#include <QFileDialog>

Canvas::Canvas(QWidget* parent) : QOpenGLWidget(parent)
{
    this->window = static_cast<Window*>(parent);
}

Canvas::~Canvas()
{
    delete terrain;
    delete camera;
    delete cameraController;
    delete noiseImage;
    delete shadowMapTechnique;
    delete light;

    Shaders::DeleteAll();
}

//---------------------------------------------------------------------------//
//----------------------- QT OpenGL Callback Functions ----------------------//
//---------------------------------------------------------------------------//

void Canvas::initializeGL()
{
    initializeOpenGLFunctions();

    lastMousePos = QCursor::pos();

    if(!Shaders::InitializeShaders())
    {
        exit(-1);
    }

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

    cameraController = new CameraController();
    cameraController->setCamera(camera);

    light = new DirectionalLight(glm::vec3(-1.0,-1.0, 0.0), glm::vec3(1.0, 1.0, 1.0));
    shadowMapTechnique = new ShadowMapTechnique(this, 1024, 1024);
    shadowMapTechnique->light = light;

    skyboxTechnique = new SkyboxTechnique(this);

    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);

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

    // Draw Scene
    draw();

    // Display last frametime
    double ms = (double)time.msecsTo(QTime::currentTime());
    window->setFPSLabel(1000.0 / ms);

}

void Canvas::resizeGL()
{
}

//---------------------------------------------------------------------------//
//-------------------------- Private Draw Functions -------------------------//
//---------------------------------------------------------------------------//

void Canvas::draw()
{
    // Render Shadow Map
    shadowMapTechnique->drawShadowMap(terrain);

    glBindFramebuffer(GL_FRAMEBUFFER, defaultFramebufferObject());
    glViewport(0,0,size().width(), size().height());
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

    // Render Skybox
    if(skyBoxEnabled)
    {
        skyboxTechnique->draw();
        glClear(GL_DEPTH_BUFFER_BIT);
    }
    // Render tesselated terrain
    drawTesselate(terrain);

    GLenum error = glGetError();
    if(error != GL_NO_ERROR)
    {
        qDebug()<< error;
    }
}

void Canvas::drawTesselate(Terrain* terrain)
{
    glCullFace(GL_BACK);

    QOpenGLShaderProgram* shader = Shaders::Find("tesselate");
    shader->bind();
    int unit = 1;

    // Max height of the terrain
    GLuint location = glGetUniformLocation(shader->programId(), "maxHeight");
    glUniform1f(location, terrain->maxHeight);

    // Heightmap texture
    location = glGetUniformLocation(shader->programId(), "heightmapTexture");
    glUniform1i(location, unit);
    terrain->heightmapTexture->bind(unit++);

    // Grass Texture
    location = glGetUniformLocation(shader->programId(), "grasTexture");
    glUniform1i(location, unit);
    terrain->grassTexture->bind(unit++);

    // Rock Texture
    location = glGetUniformLocation(shader->programId(), "rockTexture");
    glUniform1i(location, unit);
    terrain->rockTexture->bind(unit++);

    // Position of the camera in world space
    location = glGetUniformLocation(shader->programId(), "eyePosWorld");
    glUniform3fv(location, 1, glm::value_ptr(camera->getPosition()));

    // Number of tiles in the grid
    location = glGetUniformLocation(shader->programId(), "numTiles");
    glUniform2f(location, terrain->getGridRepetitionX(), terrain->getGridRepetitionY());

    // Direction of the light in world space
    location = glGetUniformLocation(shader->programId(), "lightDirection_World");
    glUniform3fv(location, 1, glm::value_ptr(light->direction));

    // ViewProjectionMatrix of the camera
    glm::mat4 viewProjectionMatrix = camera->getProjectionMatrix() * camera->getViewMatrix();

    // Bias Depth MVP Matrix for Shadow Mapping
    glm::mat4 biasMatrix = glm::mat4(
                0.5, 0.0, 0.0, 0.0,
                0.0, 0.5, 0.0, 0.0,
                0.0, 0.0, 0.5, 0.0,
                0.5, 0.5, 0.5, 1.0
                );

    glm::mat4 depthViewProjectionMatrix = biasMatrix * light->getProjectionMatrix() * light->getViewMatrix();

    // Render all tiles
    for (unsigned int i = 0; i < terrain->tiles.size(); i++)
    {
        Transform tile = terrain->tiles.at(i);

        // Coordinates of the current tile in the grid
        location = glGetUniformLocation(shader->programId(), "tileIndex");
        glUniform2fv(location,1, glm::value_ptr(terrain->tileCoordinates.at(i)));

        // MVP matrix of the current tile
        location = glGetUniformLocation(shader->programId(), "modelViewProjectionMatrix");
        glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(viewProjectionMatrix * tile.modelMatrix));

        // Model matrix of the current tile
        location = glGetUniformLocation(shader->programId(), "modelMatrix");
        glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(tile.modelMatrix));

        // Normal matrix of the current tile
        location = glGetUniformLocation(shader->programId(), "normalMatrix");
        glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(tile.getNormalMatrix()));

        // Depth MVP Matrix for Shadow Mapping
        location = glGetUniformLocation(shader->programId(), "depthMVPMatrix");
        glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(depthViewProjectionMatrix * tile.modelMatrix));

        if(shadingEnabled)
        {
            location = glGetUniformLocation(shader->programId(), "wireframeEnabled");
            glUniform1i(location, false);
            glPolygonMode(GL_FRONT, GL_FILL);
            terrain->drawTesselate();
        }

        if(wireframeEnabled)
        {
            location = glGetUniformLocation(shader->programId(), "wireframeEnabled");
            glUniform1i(location, true);

            glPolygonMode(GL_FRONT, GL_LINE);
            terrain->drawTesselate();
        }
    }
    shader->release();
}

//---------------------------------------------------------------------------//
//-------------------------- KEY AND MOUSE EVENTS ---------------------------//
//---------------------------------------------------------------------------//

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
    float factor = 0.001f * event->angleDelta().y() * mouseWheelFactor;
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

//---------------------------------------------------------------------------//
//-------------------------- PUBLIC SLOT FUNCTIONS --------------------------//
//---------------------------------------------------------------------------//

void Canvas::generateTerrainButtonClicked()
{
    generateTerrain = true;
    update();
}

void Canvas::openNoiseTextureButtonClicked()
{
    QString filename = QFileDialog::getOpenFileName(this, tr("Open Heightmap"),tr(""),tr("Image Files (*.png *.jpg *.bmp)"));

    if(!filename.isEmpty()){
        if(noiseImage != nullptr)
            delete noiseImage;
        noiseImage = new QImage(filename);
        terrain->setHeightmapTexture(noiseImage);
        window->setNoiseImage(noiseImage);
        update();
    }
}

void Canvas::generateNoiseTextureButtonClicked()
{
    generateNoise = true;
    update();
}

void Canvas::setGridRepetitionX(int value)
{
    terrain->setGridRepetitionX(value);
    mouseWheelFactor = std::max(terrain->getGridRepetitionX(), terrain->getGridRepetitionY());
    update();
}

void Canvas::setGridRepetitionY(int value)
{
    terrain->setGridRepetitionY(value);
    mouseWheelFactor = std::max(terrain->getGridRepetitionX(), terrain->getGridRepetitionY());

    update();
}

void Canvas::setHeight(double value)
{
    terrain->maxHeight = value;
    update();
}

void Canvas::setWireframeEnabled(bool enabled)
{
    wireframeEnabled = enabled;
    update();
}

void Canvas::setShadingEnabled(bool enabled)
{
    shadingEnabled = enabled;
    update();
}

void Canvas::setShadowsEnabled(bool enabled)
{
    shadowsEnabled = enabled;
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

void Canvas::setSkyBoxEnabled(bool enabled)
{
    Canvas::skyBoxEnabled = enabled;
    update();
}
