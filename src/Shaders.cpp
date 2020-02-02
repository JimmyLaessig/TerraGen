#include "shaders.h"
#include "QFileInfo"

std::map<std::string, QOpenGLShaderProgram*> Shaders::ShaderMap;// = std::map<std::string, QOpenGLShaderProgram*>();


bool Shaders::InitializeShader(const std::string& name)
{
    constexpr auto shaderPath            = "Shader/";
    constexpr auto vertexExtension       = ".vs.glsl";
    constexpr auto tessControlExtension  = ".cs.glsl";
    constexpr auto tessEvalExtension     = ".es.glsl";
    constexpr auto fragmentExtension     = ".fs.glsl";

    QOpenGLShaderProgram* shader = new QOpenGLShaderProgram();

    auto vertexShaderPath       = shaderPath + name + vertexExtension;
    auto tessControlShaderPath  = shaderPath + name + tessControlExtension;
    auto tessEvalShaderPath     = shaderPath + name + tessEvalExtension;
    auto fragmentShaderPath     = shaderPath + name + fragmentExtension;

    if(QFileInfo::exists(vertexShaderPath.c_str()))
        shader->addShaderFromSourceFile(QOpenGLShader::Vertex,vertexShaderPath.c_str());
     if(QFileInfo::exists(tessControlShaderPath.c_str()))
        shader->addShaderFromSourceFile(QOpenGLShader::TessellationControl,tessControlShaderPath.c_str());
    if(QFileInfo::exists(tessEvalShaderPath.c_str()))
        shader->addShaderFromSourceFile(QOpenGLShader::TessellationEvaluation,tessEvalShaderPath.c_str());
    if(QFileInfo::exists(fragmentShaderPath.c_str()))
        shader->addShaderFromSourceFile(QOpenGLShader::Fragment,fragmentShaderPath.c_str());

    if (shader->link())
    {
        // Remove old instance
        Shaders::deleteShader(name);
        Shaders::ShaderMap[name] = shader;
        return true;
    }
    else
    {
        qDebug("Failed to load Shader: %s" , name.c_str());
        return false;
    }
}


bool Shaders::InitializeShaders()
{

    qDebug("Initializing Shaders!");

    InitializeShader("infinite_terrain");
    InitializeShader("tessellate");
    InitializeShader("shadowmap_tesselate");
    InitializeShader("skybox");
    InitializeShader("diffuse");

    return true;
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
