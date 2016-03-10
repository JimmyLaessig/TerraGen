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
#include "skyboxtechnique.h"

#include <shaders.h>
#include "directionallight.h"


class Canvas : public QOpenGLWidget, public QOpenGLFunctions_4_4_Core
{
    Q_OBJECT
public:

    bool wireframeEnabled = true;
    bool shadingEnabled = true;
    bool skyBoxEnabled = true;
    bool dynamicLodEnabled = true;

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

    // Button Functions
    void generateTerrainButtonClicked();

    void openNoiseTextureButtonClicked();

    void generateNoiseTextureButtonClicked();

    // Terrain specific parameter
    void setGridRepetitionX(int value);

    void setGridRepetitionY(int value);

    void setHeight(double value);

    // Noise specific parameter
    void noiseFrequency1Changed(double value);

    void noiseFrequency2Changed(double value);

    void noisePersistenceChanged(double value);

    void noiseBiasChanged(double value);

    void noiseScaleChanged(double value);

    // Render specific parameter
    void setWireframeEnabled(bool enabled);

    void setShadingEnabled(bool enabled);

    void setSkyBoxEnabled(bool enabled);

    void setDynamicLodEnabled(bool enabled);

private:

    double mouseWheelFactor = 1.0;

    void draw();
    void drawTesselate(Terrain* terrain);

    DirectionalLight* light;

    SkyboxTechnique* skyboxTechnique;
    QTime time;

    Window* window;

    QPoint lastMousePos;
    bool generateTerrain = false;
    bool generateNoise = false;
    bool mouseLeftDown;
    bool mouseRightDown;

    QImage* noiseImage;

    Terrain* terrain;
    Camera* camera;
    CameraController* cameraController;

    int width;
    int height;

};

#endif // CANVAS_H
