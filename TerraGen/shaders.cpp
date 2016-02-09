#include "shaders.h"
#include <fstream>


std::map<std::string, QOpenGLShaderProgram*> Shaders::ShaderMap = std::map<std::string, QOpenGLShaderProgram*>();


Shaders::Shaders()
{
}

Shaders::~Shaders()
{
}

bool Shaders::InitializeShaders()
{

    bool error = false;
    qDebug("Initializing Shaders!");

    //----------------------------------------//
    //---------- Tesselation Shader ----------//
    //----------------------------------------//

    QOpenGLShaderProgram* tesselate = new QOpenGLShaderProgram();

    tesselate->addShaderFromSourceFile(QOpenGLShader::Vertex, "Shader/tesselate.vs.glsl");
    tesselate->addShaderFromSourceFile(QOpenGLShader::TessellationControl, "Shader/tesselate.cs.glsl");
    tesselate->addShaderFromSourceFile(QOpenGLShader::TessellationEvaluation, "Shader/tesselate.es.glsl");
    tesselate->addShaderFromSourceFile(QOpenGLShader::Fragment, "Shader/tesselate.fs.glsl");

    if (tesselate->link())
    {
        // Remove old instance
        Shaders::deleteShader("tesselate");
        Shaders::ShaderMap["tesselate"] = tesselate;
    }
    else
    {
        qDebug("Failed to load Terrain Tesselation Shader");
        error = true;
    }

    //----------------------------------------//
    //---------- Shadow Map Shader -----------//
    //----------------------------------------//

    QOpenGLShaderProgram* shadowMap = new QOpenGLShaderProgram();
    shadowMap->addShaderFromSourceFile(QOpenGLShader::Vertex, "Shader/shadowmap_tesselate.vs.glsl");
    shadowMap->addShaderFromSourceFile(QOpenGLShader::TessellationControl, "Shader/shadowmap_tesselate.cs.glsl");
    shadowMap->addShaderFromSourceFile(QOpenGLShader::TessellationEvaluation, "Shader/shadowmap_tesselate.es.glsl");
    shadowMap->addShaderFromSourceFile(QOpenGLShader::Fragment, "Shader/shadowmap_tesselate.fs.glsl");

    if (shadowMap->link())
    {
        Shaders::deleteShader("shadowMap");
        Shaders::ShaderMap["shadowMap"] = shadowMap;
    }
    else
    {
        qDebug("Failed to load Terrain ShadowMap Tesselation Shader");
        error = true;
    }

    //-----------------------------------//
    //---------- Skybox Shader ---------//
    //-----------------------------------//

    QOpenGLShaderProgram* skybox = new QOpenGLShaderProgram();
    skybox->addShaderFromSourceFile(QOpenGLShader::Vertex, "Shader/skybox.vs.glsl");
    skybox->addShaderFromSourceFile(QOpenGLShader::Fragment, "Shader/skybox.fs.glsl");

    if (skybox->link())
    {
        Shaders::deleteShader("skybox");
        Shaders::ShaderMap["skybox"] = skybox;
    }
    else
    {
        qDebug("Failed to load skybox Shader");
        error = true;
    }

    //-----------------------------------//
    //---------- Diffuse Shader ---------//
    //-----------------------------------//

    QOpenGLShaderProgram* diffuse = new QOpenGLShaderProgram();
    diffuse->addShaderFromSourceFile(QOpenGLShader::Vertex, "Shader/diffuse.vs.glsl");
    diffuse->addShaderFromSourceFile(QOpenGLShader::Fragment, "Shader/diffuse.fs.glsl");

    if (diffuse->link())
    {
        Shaders::deleteShader("diffuse");
        Shaders::ShaderMap["diffuse"] = diffuse;
    }
    else
    {
        qDebug("Failed to load diffuse Shader");
        error = true;
    }

    return !error;
}

QOpenGLShaderProgram* Shaders::Find(std::string name)
{
    return ShaderMap[name];
}

void Shaders::deleteShader(std::string key)
{
    auto it = ShaderMap.find(key);
    if(it !=ShaderMap.end())
    {
        delete it->second;
        ShaderMap.erase(it);
    }
}

void Shaders::DeleteAll()
{
    for (auto &it : ShaderMap)
    {
        delete it.second;
    }
    ShaderMap.clear();
}
