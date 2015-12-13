#ifndef CANVAS_H
#define CANVAS_H

#pragma once

#include <QOpenGLWidget>
#include <QOpenGLFunctions_4_4_Core>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include "renderer.h"

#include "terrain.h"
#include <map>
#include <QKeyEvent>
#include "glm/vec2.hpp"
#include "camera.h"


class Canvas : public QOpenGLWidget, public QOpenGLFunctions_4_4_Core
{
    Q_OBJECT
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

public slots:

    void dimXValueChanged(int value);

    void dimYValueChanged(int value);

    void generateTerrainButtonClicked();

    void openNoiseTextureButtonClicked();

    void noiseTypeChanged(QString type);

    void generateNoiseTextureButtonClicked();

    void heightValueChanged(double value);

    void wireframeEnabled(bool enabled);

    void shadingEnabled(bool enabled);

    void dynamicLoDEnabled(bool enabled);

    void textureRepeatValueChanged(double value);

    void dynamicTexturingEnabled(bool enabled);

    void normalMappingEnabled(bool enabled);

    void shadowsEnabled(bool enabled);

    void distanceFogEnabled(bool enabled);

private:

    Renderer* renderer;

    glm::vec2 lasMousePos;
    bool generateTerrain = false;
    bool generateNoise = false;
    bool mouseLeftDown;
    bool mouseRightDown;

    Terrain* terrain;
    Camera* camera;

};

#endif // CANVAS_H
