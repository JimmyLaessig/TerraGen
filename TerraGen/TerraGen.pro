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
    perlinnoisegenerator.cpp \
    renderer.cpp \
    sceneobject.cpp \
    shaders.cpp \
    simplexnoise.cpp \
    simplexnoisegenerator.cpp \
    terrain.cpp \
    terraingenerator.cpp \
    window.cpp


FORMS    += window.ui

HEADERS += \
    camera.h \
    canvas.h \
    perlinnoisegenerator.h \
    renderer.h \
    sceneobject.h \
    shaders.h \
    simplexnoise.h \
    simplexnoisegenerator.h \
    terrain.h \
    terraingenerator.h \
    window.h

DISTFILES +=\
    ../Shader/tesselate.vs.glsl \
    ../Shader/tesselate.cs.glsl \
    ../Shader/tesselate.es.glsl \
    ../Shader/tesselate.fs.glsl \
    ../Shader/diffuse.vs.glsl \
    ../Shader/diffuse.fs.glsl



INCLUDEPATH += $$PWD/../External/Include
