#ifndef CANVAS_H
#define CANVAS_H

#pragma once

#include <QOpenGLWidget>
#include <QOpenGLFunctions_4_4_Core>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QKeyEvent>
#include "glm/vec2.hpp"
#include "window.h"
#include "renderer.h"
#include "terrain.h"
#include "simplexnoisegenerator.h"
#include "perlinnoisegenerator.h"
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

    void noiseOctavsChanged(double value);

    void noisePersistenceChanged(double value);

    void noiseScaleChanged(double value);

private:

    Window* window;

    Renderer* renderer;

    glm::vec2 lasMousePos;
    bool generateTerrain = false;
    bool generateNoise = false;
    bool mouseLeftDown;
    bool mouseRightDown;

    QImage* noiseImage;

    Terrain* terrain;
    Camera* camera;

};

#endif // CANVAS_H
