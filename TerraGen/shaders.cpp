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

    QOpenGLShaderProgram* diffuse = new QOpenGLShaderProgram();
    diffuse->addShaderFromSourceFile(QOpenGLShader::Vertex, "../Shader/diffuse.vert");
    diffuse->addShaderFromSourceFile(QOpenGLShader::Fragment, "../Shader/diffuse.frag");

    if (diffuse->link())
	{
        Shaders::ShaderMap["diffuse"] = diffuse;
	}
    else
    {
       qDebug("Failed to load Shader Diffuse");
       exit(-1);
    }
}

QOpenGLShaderProgram* Shaders::Find(std::string name)
{
    return Shaders::ShaderMap.at(name);
}

void Shaders::DeleteAll()
{
//	for (auto &it : Shader::Shaders)
//	{
//		delete it.second;
//	}
//	Shader::Shaders.clear();
}
