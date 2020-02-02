#include "Canvas.h"
#include <QTime>
#include "Shaders.h"
#include <QKeyEvent>
#include "glm/gtc/type_ptr.hpp"
#include <iostream>
#include <QDebug>
#include <QElapsedTimer>
#include "IndexedGeometry.h"
#include "heightmapgenerator.h"

#include <algorithm>
#include <QFileDialog>
#include <QtOpenGLExtensions/QOpenGLExtensions>

QOpenGLFunctions_4_5_Core* Canvas::OpenGLFunctions = nullptr;


Canvas::Canvas(QWidget* parent) :
    QOpenGLWidget(parent),
    window(static_cast<Window*>(parent)),
    camera(),
    light(glm::vec3(0, 0,0), glm::normalize(glm::vec3(0,-0.5, 1.0)), glm::vec3(1.0, 1.0, 1.0))
{
    OpenGLFunctions = this;
}


Canvas::~Canvas()
{
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

   // noiseImage = QImage(512, 512, QImage::Format_RGB16);
   // noiseImage.fill(qRgb(255, 255, 255));
   // window->setNoiseImage(&noiseImage);

    this->cube = std::unique_ptr<IndexedGeometry>(IndexedGeometry::Cube());

    cube->createVAO();
    terrain.initializeGL();
    //terrain.setHeightmapTexture(&noiseImage);

    float ratio = float(size().width())/ float(size().height());
    this->camera.setProjectionMatrix(70.0f, ratio, 0.1f, 10000.0f);
    this->camera.lookAt(glm::vec3(-5, 0, 1), glm::vec3(0, 0, 0));

    //camera->rotate(glm::vec3(0, 1, 0), 45);
    //camera->rotate(glm::vec3(1, 0, 0), 45);

    cameraController.setCamera(&camera);

   // shadowMapTechnique = new ShadowMapTechnique(this, 1024, 1024);
   // shadowMapTechnique->light = light;

    //skyboxTechnique = new SkyboxTechnique(this);

    glDisable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);

}

void Canvas::paintGL()
{
    QTime time = QTime::currentTime();

    // Draw Scene
    glBindFramebuffer(GL_FRAMEBUFFER, defaultFramebufferObject());
    glViewport(0,0,size().width(), size().height());
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    auto cameraPosition             = camera.transform.getPosition();
    auto viewMatrix                 = camera.getViewMatrix();
    auto projMatrix                 = camera.getProjectionMatrix();
    auto viewProjectionMatrix       = projMatrix * viewMatrix;
    auto invViewProjectionMatrix    = glm::inverse(viewProjectionMatrix);
    auto invViewMatrix              = glm::inverse(viewMatrix);
    auto invProjMatrix              = glm::inverse(projMatrix);

    // Render Skybox
    if(skyBoxEnabled)
    {
        //skyboxTechnique->draw();
    }
    {
//        QOpenGLShaderProgram* shader = Shaders::Find("diffuse");
//        auto worldMatrix = cubeTransform.getLocalToWorldMatrix();
//        auto normalMatrix = cubeTransform.getLocalToWorldNormalMatrix();
//        shader->bind();

//        glUniformMatrix4fv(shader->uniformLocation("worldMatrix"),                  1, GL_FALSE, glm::value_ptr(worldMatrix));
//        glUniformMatrix4fv(shader->uniformLocation("worldViewProjectionMatrix"),    1, GL_FALSE, glm::value_ptr(viewProjectionMatrix * worldMatrix));
//        glUniformMatrix3fv(shader->uniformLocation("normalMatrix"),                 1, GL_FALSE, glm::value_ptr(normalMatrix));
//        glBindVertexArray(cube->VAO);
//        glDrawElements(GL_TRIANGLES,  GLsizei(cube->indices.size()), GL_UNSIGNED_INT, nullptr);
    }
    {
        std::vector<glm::mat4>instanceMatrices;
        auto positions = terrain.computeVisibleTilePositions(camera);
        std::transform(std::begin(positions), std::end(positions), std::back_inserter(instanceMatrices), [](const glm::vec3& pos)
        {
            return glm::translate(glm::mat4(1), pos);
        });


        glBindVertexArray(terrain.grid->VAO);

        unsigned int instanceVBO;
        glGenBuffers(1, &instanceVBO);

        glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
        glBufferData(GL_ARRAY_BUFFER, GLsizei(instanceMatrices.size() * sizeof(glm::mat4)), glm::value_ptr(instanceMatrices[0]), GL_STATIC_DRAW);
        glEnableVertexAttribArray(2);
        glEnableVertexAttribArray(3);
        glEnableVertexAttribArray(4);
        glEnableVertexAttribArray(5);

        glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)0);
        glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(1 * sizeof(glm::vec4)));
        glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(2 * sizeof(glm::vec4)));
        glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(3 * sizeof(glm::vec4)));

        glVertexAttribDivisor(2, 1);
        glVertexAttribDivisor(3, 1);
        glVertexAttribDivisor(4, 1);
        glVertexAttribDivisor(5, 1);


        QOpenGLShaderProgram* shader = Shaders::Find("infinite_terrain");
        shader->bind();
        shader->setPatchVertexCount(3);
        glPatchParameteri(GL_PATCH_VERTICES, 3);
        glUniform3fv(shader->uniformLocation("cameraPosition"),                 1, glm::value_ptr(cameraPosition));
        glUniformMatrix4fv(shader->uniformLocation("worldMatrix"),             1, GL_FALSE, glm::value_ptr(glm::mat4(1)));
        glUniformMatrix4fv(shader->uniformLocation("worldViewProjectionMatrix"), 1, GL_FALSE, glm::value_ptr(viewProjectionMatrix));

        glUniformMatrix4fv(shader->uniformLocation("viewMatrix"),               1, GL_FALSE, glm::value_ptr(viewMatrix));
        glUniformMatrix4fv(shader->uniformLocation("projectionMatrix"),         1, GL_FALSE, glm::value_ptr(projMatrix));
        glUniformMatrix4fv(shader->uniformLocation("viewProjectionMatrix"),     1, GL_FALSE, glm::value_ptr(viewProjectionMatrix));
        glUniformMatrix4fv(shader->uniformLocation("invViewProjectionMatrix"),  1, GL_FALSE, glm::value_ptr(invViewProjectionMatrix));
        glUniformMatrix4fv(shader->uniformLocation("invProjectionMatrix"),      1, GL_FALSE, glm::value_ptr(invProjMatrix));
        glUniformMatrix4fv(shader->uniformLocation("invViewMatrix"),            1, GL_FALSE, glm::value_ptr(invViewMatrix));
        {
            glUniform1i(shader->uniformLocation("bWireframeEnabled"), 0);
            glPolygonMode(GL_FRONT, GL_FILL);
            glPolygonOffset(0.0, 0.0);
            glEnable(GL_DEPTH_TEST);
            glDepthFunc(GL_LESS);
            glDrawElementsInstanced(GL_PATCHES,  GLsizei(terrain.grid->indices.size()), GL_UNSIGNED_INT, nullptr, GLsizei(instanceMatrices.size()));
        }
        {
            glUniform1i(shader->uniformLocation("bWireframeEnabled"), 1);
            glDepthFunc(GL_LEQUAL);
            glPolygonMode(GL_FRONT, GL_LINE);
            glPolygonOffset(0.1f, 0.1f);
            //glDisable(GL_DEPTH_TEST);
            glDrawElementsInstanced(GL_PATCHES,  GLsizei(terrain.grid->indices.size()), GL_UNSIGNED_INT, nullptr, GLsizei(instanceMatrices.size()));
        }
        glDeleteBuffers(1, &instanceVBO);
}
    // MVP matrix of the current tile




//    auto invViewProjectionMatrix    = glm::inverse(viewProjectionMatrix);
//    glUniformMatrix4fv(shader->uniformLocation("invViewProjectionMatrix"), 1, GL_FALSE, glm::value_ptr(invViewProjectionMatrix));
//    glUniformMatrix4fv(shader->uniformLocation("viewProjectionMatrix"), 1, GL_FALSE, glm::value_ptr(viewProjectionMatrix));
//    glUniform3fv(shader->uniformLocation("cameraPosition"), 1, glm::value_ptr(camera.transform.getPosition()));


//    if(shadingEnabled)
//    {
//        //  glEnable(GL_DEPTH_TEST);
//        //glUniform1i(shader->uniformLocation("wireframeEnabled"), false);
//        shader->setUniformValue("wireframeEnabled", false);
//        glPolygonMode(GL_FRONT, GL_FILL);
//        terrain.renderTiles(camera, *shader);
//    }

//    if(wireframeEnabled)
//    {
//       // glDisable(GL_DEPTH_TEST);
//        shader->setUniformValue("wireframeEnabled", true);
//        //glUniform1i(shader->uniformLocation("wireframeEnabled"), true);
//        glPolygonMode(GL_FRONT, GL_LINE);
//        terrain.renderTiles(camera, *shader);
//    }

//    shader->release();

    GLenum error = glGetError();
    if(error != GL_NO_ERROR)
    {
        qDebug()<<"OpenGLError: "<< error;
    }

    // Display last frametime
    double ms = double(time.msecsTo(QTime::currentTime()));
    window->setFPSLabel(1000.0 / ms);
}


void Canvas::resizeGL()
{
    update();
}


//---------------------------------------------------------------------------//
//-------------------------- KEY AND MOUSE EVENTS ---------------------------//
//---------------------------------------------------------------------------//

void Canvas::mouseMoveEvent(QMouseEvent * event)
{
    if(event->buttons() == Qt::RightButton)
    {
        glm::vec2 delta
        (
            (QCursor::pos().x() - lastMousePos.x()) / float(size().width()),
            (QCursor::pos().y() - lastMousePos.y()) / float(size().width())
        );
        cameraController.mouseMoved(-delta);
        update();
    }
    lastMousePos = QCursor::pos();
}

void Canvas::wheelEvent(QWheelEvent * event)
{
    //float factor = 0.001f * event->angleDelta().y() * mouseWheelFactor;
    //camera->translate( camera->forward()* factor);
    update();
}

void Canvas::keyPressEvent(QKeyEvent* event)
{
    cameraController.keyPressed(event->key());
    update();
}

void Canvas::keyReleaseEvent(QKeyEvent *event)
{
    if(event->isAutoRepeat())
        return;

    cameraController.keyReleased(event->key());

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
   // generateTerrain = true;
    update();
}

void Canvas::openNoiseTextureButtonClicked()
{
//    QString filename = QFileDialog::getOpenFileName(this, tr("Open Heightmap"),tr(""),tr("Image Files (*.png *.jpg *.bmp)"));

//    if(!filename.isEmpty())
//    {
//        noiseImage = QImage(filename);
//        //terrain.setHeightmapTexture(&noiseImage);
//        window->setNoiseImage(&noiseImage);
//        update();
//    }
}


void Canvas::generateNoiseTextureButtonClicked()
{
 //   generateNoise = true;
    update();
}


void Canvas::setGridRepetitionX(int value)
{
    //terrain.setGridRepetitionX(value);
   // mouseWheelFactor = std::max(terrain.getGridRepetitionX(), terrain.getGridRepetitionY());
    update();
}


void Canvas::setGridRepetitionY(int value)
{
   // terrain.setGridRepetitionY(value);
    //mouseWheelFactor = std::max(terrain.getGridRepetitionX(), terrain.getGridRepetitionY());
    update();
}


void Canvas::setHeight(double value)
{
  //  terrain.maxHeight = value;
    update();
}


void Canvas::setWireframeEnabled(bool enabled)
{
 //   wireframeEnabled = enabled;
    update();
}


void Canvas::setShadingEnabled(bool enabled)
{
 //   shadingEnabled = enabled;
    update();
}


void Canvas::setDynamicLodEnabled(bool enabled)
{
 //   dynamicLodEnabled = enabled;
    update();
}


void Canvas::noiseFrequency1Changed(double value)
{
 //   HeightmapGenerator::Frequency1 = value;
}


void Canvas::noiseFrequency2Changed(double value)
{
 //   HeightmapGenerator::Frequency2 = value;
}


void Canvas::noisePersistenceChanged(double value)
{
 //   HeightmapGenerator::Persistence = value;
}


void Canvas::noiseBiasChanged(double value)
{
//    HeightmapGenerator::Bias = value;
}


void Canvas::noiseScaleChanged(double value)
{
 //   HeightmapGenerator::Scale = value;
}


void Canvas::setSkyBoxEnabled(bool enabled)
{
//    Canvas::skyBoxEnabled = enabled;
    update();
}
