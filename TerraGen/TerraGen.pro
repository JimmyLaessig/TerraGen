#-------------------------------------------------
#
# Project created by QtCreator 2015-12-14T22:05:33
#
#-------------------------------------------------

QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = TerraGen
TEMPLATE = app


SOURCES += main.cpp\
    camera.cpp \
    canvas.cpp \
    renderer.cpp \
    shaders.cpp \
    terrain.cpp \
    window.cpp \
    transform.cpp \
    gridgenerator.cpp \
    cameracontroller.cpp \
    noiseutils.cpp \
    heightmapgenerator.cpp


FORMS    += window.ui

HEADERS += \
    camera.h \
    canvas.h \
    renderer.h \
    shaders.h \
    terrain.h \
    window.h \
    transform.h \
    gridgenerator.h \
    cameracontroller.h \
    noiseutils.h \
    heightmapgenerator.h

DISTFILES +=\
    ../Shader/tesselate.vs.glsl \
    ../Shader/tesselate.cs.glsl \
    ../Shader/tesselate.es.glsl \
    ../Shader/tesselate.fs.glsl \
    ../Shader/diffuse.vs.glsl \
    ../Shader/diffuse.fs.glsl



INCLUDEPATH += $$PWD/../External/Include

win32: LIBS += -L$$PWD/../External/Lib/ -llibnoise

INCLUDEPATH += $$PWD/../External/Include
DEPENDPATH += $$PWD/../External/Include
