#pragma once

#include <QOpenGLWidget>
//#include <QOpenGLFunctions_4_4_Core>
#include <QOpenGLFunctions_4_5_Core>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QKeyEvent>

#include "Window.h"
#include "InfiniteTerrain.h"

#include "CameraController.h"
#include "skyboxtechnique.h"

#include "Shaders.h"
#include "DirectionalLight.h"


class Canvas : public QOpenGLWidget, public QOpenGLFunctions_4_5_Core
{
    Q_OBJECT
public:
    static QOpenGLFunctions_4_5_Core* OpenGLFunctions;

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

    void generateTerrainButtonClicked();

    void openNoiseTextureButtonClicked();

    void generateNoiseTextureButtonClicked();

    void setGridRepetitionX(int value);

    void setGridRepetitionY(int value);

    void setHeight(double value);

    void noiseFrequency1Changed(double value);

    void noiseFrequency2Changed(double value);

    void noisePersistenceChanged(double value);

    void noiseBiasChanged(double value);

    void noiseScaleChanged(double value);

    void setWireframeEnabled(bool enabled);

    void setShadingEnabled(bool enabled);

    void setSkyBoxEnabled(bool enabled);

    void setDynamicLodEnabled(bool enabled);

private:

    Window* window;

    InfiniteTerrain terrain;

    Camera camera;

    CameraController cameraController;

    DirectionalLight light;

    std::unique_ptr<IndexedGeometry> cube;

    Transform cubeTransform;

    //SkyboxTechnique skyboxTechnique;

   // QImage noiseImage;

    QPoint lastMousePos;

//    bool generateTerrain    = false;
//    bool generateNoise      = false;
//    bool mouseLeftDown      = false;
//    bool mouseRightDown     = false;

    bool wireframeEnabled   = true;
    bool shadingEnabled     = true;
    bool skyBoxEnabled      = true;
    bool dynamicLodEnabled  = true;

  //  double mouseWheelFactor = 1.0;
};
