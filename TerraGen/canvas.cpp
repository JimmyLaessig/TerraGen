#include "canvas.h"
#include "QMatrix4x4"
#include "shaders.h"
#include <QKeyEvent>
#include "glm/gtc/type_ptr.hpp"
#include <iostream>
#include "terraingenerator.h"
Canvas::Canvas(QWidget* parent) : QOpenGLWidget(parent)
{
    bool leftMouseDown = false;
    bool rightMouseDown = false;
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
    camera = new Camera();
    float ratio = (float)this->size().width()/ (float)this->size().height();
    camera->setProjectionMatrix(60.0f, ratio, 0.1f, 1000.0f);
    camera->translate(glm::vec3(0, 2, 0));
    camera->rotate(glm::vec3(1,0,0), 90.0);
}

void Canvas::paintGL()
{
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glViewport(0,0,size().width(), size().height());
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    QOpenGLShaderProgram* shader = Shaders::Find("tesselate");

    shader->bind();

    glm::mat4 modelViewProjectionMatrix = camera->getProjectionMatrix() * camera->getViewMatrix() *terrain->modelMatrix;

    GLuint location = glGetUniformLocation(shader->programId(), "modelViewProjectionMatrix");
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(modelViewProjectionMatrix));

    terrain->draw();
    shader->release();
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



