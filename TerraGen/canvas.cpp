#include "canvas.h"
#include "QMatrix4x4"
#include "shaders.h"
#include <QKeyEvent>
#include "glm/gtc/type_ptr.hpp"
#include <iostream>
#include "terraingenerator.h"


Canvas::Canvas(QWidget* parent) : QOpenGLWidget(parent)
{
    mouseLeftDown = false;
    mouseRightDown = false;

    drawGrid = true;
    drawTesselate = true;
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

    terrain = TerrainGenerator::Generate(this);

    texture = new QOpenGLTexture(QImage("../Assets/test2.png"));
    qDebug(std::to_string(texture->height()).data());
    qDebug(std::to_string(texture->width()).data());


    camera = new Camera();
    float ratio = (float)this->size().width()/ (float)this->size().height();
    camera->setProjectionMatrix(70.0f, ratio, 0.1f, 1000.0f);
    camera->translate(glm::vec3(0, 2, 0));
    camera->rotate(glm::vec3(1,0,0), 90.0f);
}

void Canvas::paintGL()
{

    glViewport(0,0,size().width(), size().height());
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

    if(drawGrid)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        QOpenGLShaderProgram* shader = Shaders::Find("diffuse");
        shader->bind();

        texture->textureId();

        glUniform1i(glGetUniformLocation(shader->programId(), "colorTexture"), 1);
        glActiveTexture(GL_TEXTURE0 + 1);
        glBindTexture(GL_TEXTURE_2D, texture->textureId());

        glm::mat4 modelViewProjectionMatrix = camera->getProjectionMatrix() * camera->getViewMatrix() *terrain->modelMatrix;
        GLuint location = glGetUniformLocation(shader->programId(), "modelViewProjectionMatrix");
        glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(modelViewProjectionMatrix));

        terrain->drawGrid();
        shader->release();
    }

    if(drawTesselate)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        QOpenGLShaderProgram* shader = Shaders::Find("tesselate");
        shader->bind();

        GLuint location = glGetUniformLocation(shader->programId(), "modelMatrix");
        glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(terrain->modelMatrix));

        glm::mat4 viewProjectionMatrix = camera->getProjectionMatrix() * camera->getViewMatrix() ;
        location = glGetUniformLocation(shader->programId(), "viewProjectionMatrix");
        glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(viewProjectionMatrix));

        location = glGetUniformLocation(shader->programId(), "eyePosWorld");
        glm::vec3 eyePosWorld = camera->getPosition();
        glUniform3f(location, eyePosWorld.x, eyePosWorld.y, eyePosWorld.z);

        terrain->drawTesselate();
        shader->release();
    }


    GLenum error = glGetError();
    if(error != GL_NO_ERROR)
    {
        qDebug()<< error;
    }
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



