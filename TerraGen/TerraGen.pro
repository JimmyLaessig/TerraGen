#-------------------------------------------------
#
# Project created by QtCreator 2015-12-07T18:54:05
#
#-------------------------------------------------

QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = TerraGen
TEMPLATE = app


SOURCES += main.cpp\
    window.cpp \
    canvas.cpp \
    sceneobject.cpp \
    terrain.cpp \
    shaders.cpp \
    camera.cpp

HEADERS  += window.h\
    canvas.h \
    renderer.h \
    sceneobject.h \
    terrain.h \
    shaders.h \
    camera.h

FORMS    += window.ui

DISTFILES += \
    ../Shader/diffuse.frag \
    ../Shader/diffuse.vert


INCLUDEPATH += $$PWD/../External/Include
