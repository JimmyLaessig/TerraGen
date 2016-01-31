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
#include "heightmapgenerator.h"
#include "camera.h"
#include "cameracontroller.h"


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

    virtual void wheelEvent(QWheelEvent * event);

    virtual void keyPressEvent(QKeyEvent * event);

    virtual void keyReleaseEvent(QKeyEvent * event);

public slots:

    void gridRepetitionXChanged(int value);

    void gridRepetitionYChanged(int value);

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

    void noiseFrequency1Changed(double value);

    void noiseFrequency2Changed(double value);

    void noisePersistenceChanged(double value);

    void noiseBiasChanged(double value);

    void noiseScaleChanged(double value);

private:

    QTime time;

    Window* window;

    Renderer* renderer;

    QPoint lastMousePos;
    bool generateTerrain = false;
    bool generateNoise = false;
    bool mouseLeftDown;
    bool mouseRightDown;

    QImage* noiseImage;

    Terrain* terrain;
    Camera* camera;
    CameraController* cameraController;

};

#endif // CANVAS_H
