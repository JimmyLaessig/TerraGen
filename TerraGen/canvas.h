#ifndef CANVAS_H
#define CANVAS_H

#pragma once

#include <QOpenGLWidget>
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLShaderProgram>
#include "terrain.h"
#include <map>
#include <QKeyEvent>
#include "glm/vec2.hpp"
#include "camera.h"


class Canvas : public QOpenGLWidget, public QOpenGLFunctions_3_3_Core
{
public:
    Canvas(QWidget* parent);
    ~Canvas();

    virtual void initializeGL();

    virtual void paintGL();

    virtual void resizeGL();

    virtual void mouseMoveEvent(QMouseEvent * event);

    virtual void mousePressEvent(QMouseEvent * event);

    virtual void mouseReleaseEvent(QMouseEvent * event);

    virtual void wheelEvent(QWheelEvent * event);

    virtual void keyPressEvent(QKeyEvent * event);

private:
    glm::vec2 lasMousePos;

    bool mouseLeftDown;
    bool mouseRightDown;

    Terrain* terrain;
    Camera* camera;
    void initializeShaders();

    QOpenGLShaderProgram diffuseShader;
};

#endif // CANVAS_H
