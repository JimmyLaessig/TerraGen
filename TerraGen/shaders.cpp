#include "shaders.h"
#include <fstream>


std::map<std::string, QOpenGLShaderProgram*> Shaders::ShaderMap = std::map<std::string, QOpenGLShaderProgram*>();


Shaders::Shaders()
{
}

Shaders::~Shaders()
{
}

void Shaders::InitializeShaders()
{
    qDebug("Loading Shaders!");

    // TODO Load all shaders here at once

    QOpenGLShaderProgram* tesselate = new QOpenGLShaderProgram();
    tesselate->addShaderFromSourceFile(QOpenGLShader::Vertex, "Shader/tesselate.vs.glsl");
    tesselate->addShaderFromSourceFile(QOpenGLShader::TessellationControl, "Shader/tesselate.cs.glsl");
    tesselate->addShaderFromSourceFile(QOpenGLShader::TessellationEvaluation, "Shader/tesselate.es.glsl");
    tesselate->addShaderFromSourceFile(QOpenGLShader::Fragment, "Shader/tesselate.fs.glsl");

    if (tesselate->link())
    {
        Shaders::ShaderMap["tesselate"] = tesselate;
    }
    else
    {
        qDebug("Failed to load Terrain Tesselation Shader");
        exit(-1);
    }

    QOpenGLShaderProgram* shadowMap = new QOpenGLShaderProgram();
    shadowMap->addShaderFromSourceFile(QOpenGLShader::Vertex, "Shader/shadowmap_tesselate.vs.glsl");
    shadowMap->addShaderFromSourceFile(QOpenGLShader::TessellationControl, "Shader/shadowmap_tesselate.cs.glsl");
    shadowMap->addShaderFromSourceFile(QOpenGLShader::TessellationEvaluation, "Shader/shadowmap_tesselate.es.glsl");
    shadowMap->addShaderFromSourceFile(QOpenGLShader::Fragment, "Shader/shadowmap_tesselate.fs.glsl");

    if (shadowMap->link())
    {
        Shaders::ShaderMap["shadowMap"] = shadowMap;
    }
    else
    {
        qDebug("Failed to load Terrain ShadowMap Tesselation Shader");
        exit(-1);
    }

    QOpenGLShaderProgram* diffuse = new QOpenGLShaderProgram();
    diffuse->addShaderFromSourceFile(QOpenGLShader::Vertex, "Shader/diffuse.vs.glsl");
    diffuse->addShaderFromSourceFile(QOpenGLShader::Fragment, "Shader/diffuse.fs.glsl");

    if (diffuse->link())
    {
        Shaders::ShaderMap["diffuse"] = diffuse;
    }
    else
    {
        qDebug("Failed to load diffuse Shader");
        exit(-1);
    }
}

QOpenGLShaderProgram* Shaders::Find(std::string name)
{
    return Shaders::ShaderMap.at(name);
}

void Shaders::DeleteAll()
{
        for (auto &it : ShaderMap)
        {
            delete it.second;
        }
        ShaderMap.clear();
}
