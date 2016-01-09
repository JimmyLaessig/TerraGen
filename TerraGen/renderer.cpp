#include "renderer.h"
#include "glm/gtc/type_ptr.hpp"

Renderer::Renderer(QOpenGLFunctions_4_4_Core* functions, int width, int height)
{
    this->functions = functions;
    this->width = width;
    this->height = height;

    functions->glEnable(GL_DEPTH_TEST);
    functions->glEnable(GL_CULL_FACE);
    functions->glCullFace(GL_BACK);
    functions->glDepthMask(GL_TRUE);

    // TODO Initialize framebuffer here
}
Renderer::~Renderer()
{

}

void Renderer::paintGL(Terrain* terrain)
{

    functions->glViewport(0,0,width, height);
    functions->glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    functions->glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

    if(dynamicLoDEnabled)
    {
        drawTesselate(terrain);
    }
    else
    {
        drawSimple(terrain);
    }


    GLenum error = functions->glGetError();
    if(error != GL_NO_ERROR)
    {
        qDebug()<< error;
    }
}


void Renderer::drawTesselate(Terrain* terrain)
{

    QOpenGLShaderProgram* shader = Shaders::Find("tesselate");
    shader->bind();

    int gridRepetitionsX = terrain->getGridRepetitionX();
    int gridRepetitionsY = terrain->getGridRepetitionY();

    GLuint location = functions->glGetUniformLocation(shader->programId(), "heightmapTexture");
    functions->glUniform1i(location, 1);
    functions->glActiveTexture(GL_TEXTURE0 + 1);
    functions->glBindTexture(GL_TEXTURE_2D, terrain->heightmapTexture->textureId());

    location = functions->glGetUniformLocation(shader->programId(), "heightScale");
    functions->glUniform1f(location, terrain->maxHeight);

    location = functions->glGetUniformLocation(shader->programId(), "colorTexture");
    functions->glUniform1i(location, 2);
    functions->glActiveTexture(GL_TEXTURE0 + 2);
    functions->glBindTexture(GL_TEXTURE_2D, terrain->colorTexture->textureId());

    location = functions->glGetUniformLocation(shader->programId(), "texcoordScale");
    functions->glUniform1f(location, terrain->texcoordScale);

    glm::mat4 viewProjectionMatrix = camera->getProjectionMatrix() * camera->getViewMatrix();


    location = functions->glGetUniformLocation(shader->programId(), "eyePosWorld");
    glm::vec3 eyePosWorld = camera->getPosition();
    functions->glUniform3f(location, eyePosWorld.x, eyePosWorld.y, eyePosWorld.z);

    location = functions->glGetUniformLocation(shader->programId(), "gridDimensions");
    functions->glUniform2f(location, gridRepetitionsX, gridRepetitionsY);

    for (unsigned int i = 0; i < terrain->transforms.size(); i++)
    {
        glm::vec2 gridCoords = terrain->gridCoords.at(i);
        location = functions->glGetUniformLocation(shader->programId(), "gridCoords");
        functions->glUniform2f(location, gridCoords.x, gridCoords.y);

        location = functions->glGetUniformLocation(shader->programId(), "modelViewProjectionMatrix");
        functions->glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(viewProjectionMatrix * terrain->transforms.at(i).modelMatrix));

        if(shadingEnabled)
        {
            location = functions->glGetUniformLocation(shader->programId(), "wireframeEnabled");
            functions->glUniform1i(location, false);
            functions->glPolygonMode(GL_FRONT, GL_FILL);
            terrain->drawTesselate();
        }

        if(wireframeEnabled)
        {
            location = functions->glGetUniformLocation(shader->programId(), "wireframeEnabled");
            functions->glUniform1i(location, true);

            functions->glPolygonMode(GL_FRONT, GL_LINE);
            terrain->drawTesselate();
        }

    }
    shader->release();
}

void Renderer::drawSimple(Terrain* terrain)
{

    QOpenGLShaderProgram* shader = Shaders::Find("diffuse");
    shader->bind();

    for(unsigned int i = 0; i < terrain->transforms.size(); i++)
    {

        glm::mat4 modelViewProjectionMatrix = camera->getProjectionMatrix() * camera->getViewMatrix() *terrain->transforms.at(i).modelMatrix;
        GLuint location = functions->glGetUniformLocation(shader->programId(), "modelViewProjectionMatrix");
        functions->glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(modelViewProjectionMatrix));

        if(shadingEnabled)
        {
            location = functions->glGetUniformLocation(shader->programId(), "colorTexture");
            functions->glUniform1i(location, 1);
            functions->glActiveTexture(GL_TEXTURE0 + 1);
            functions->glBindTexture(GL_TEXTURE_2D, terrain->colorTexture->textureId());

            location = functions->glGetUniformLocation(shader->programId(), "texcoordScale");
            functions->glUniform1f(location, terrain->texcoordScale);

            location = functions->glGetUniformLocation(shader->programId(), "wireframeEnabled");
            functions->glUniform1i(location, false);
            functions->glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            terrain->drawSimple();
        }

        if(wireframeEnabled)
        {
            location = functions->glGetUniformLocation(shader->programId(), "wireframeEnabled");
            functions->glUniform1i(location, true);

            functions->glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            terrain->drawSimple();
        }

    }
    shader->release();
}

void Renderer::resizeGL()
{
    // TODO RESIZE FRAMEBUFFERS HERE
}

