#include "renderer.h"
#include "glm/gtc/type_ptr.hpp"

Renderer::Renderer(QOpenGLFunctions_4_4_Core* functions, int width, int height)
{
    this->functions = functions;
    this->width = width;
    this->height = height;

    light = new DirectionalLight(glm::vec3(-1,-1, 0), glm::vec3(1, 1, 1));

    functions->glEnable(GL_DEPTH_TEST);
    functions->glEnable(GL_CULL_FACE);
    functions->glCullFace(GL_BACK);
    functions->glDepthMask(GL_TRUE);

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
    int unit = 1;

    // Max height of the terrain
    GLuint location = functions->glGetUniformLocation(shader->programId(), "maxHeight");
    functions->glUniform1f(location, terrain->maxHeight);

    // Heightmap texture
    location = functions->glGetUniformLocation(shader->programId(), "heightmapTexture");
    functions->glUniform1i(location, unit);
    terrain->heightmapTexture->bind(unit++);

    // Grass Texture
    location = functions->glGetUniformLocation(shader->programId(), "grasTexture");
    functions->glUniform1i(location, unit);
    terrain->grassTexture->bind(unit++);

    // Rock Texture
    location = functions->glGetUniformLocation(shader->programId(), "rockTexture");
    functions->glUniform1i(location, unit);
    terrain->rockTexture->bind(unit++);

    // Position of the camera in world space
    location = functions->glGetUniformLocation(shader->programId(), "eyePosWorld");
    functions->glUniform3fv(location, 1, glm::value_ptr(camera->getPosition()));

    // Number of tiles in the grid
    location = functions->glGetUniformLocation(shader->programId(), "numTiles");
    functions->glUniform2f(location, terrain->getGridRepetitionX(), terrain->getGridRepetitionY());

    // Direction of the light in world space
    location = functions->glGetUniformLocation(shader->programId(), "lightDirection_World");
    functions->glUniform3fv(location, 1, glm::value_ptr(light->direction));

    // ViewProjectionMatrix of the camera
    glm::mat4 viewProjectionMatrix = camera->getProjectionMatrix() * camera->getViewMatrix();

    // Render all tiles
    for (unsigned int i = 0; i < terrain->tiles.size(); i++)
    {
        Transform tile = terrain->tiles.at(i);

        // Coordinates of the current tile in the grid
        location = functions->glGetUniformLocation(shader->programId(), "tileIndex");
        functions->glUniform2fv(location,1, glm::value_ptr(terrain->tileCoordinates.at(i)));

        // MVP matrix of the current tile
        location = functions->glGetUniformLocation(shader->programId(), "modelViewProjectionMatrix");
        functions->glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(viewProjectionMatrix * tile.modelMatrix));

        // Model matrix of the current tile
        location = functions->glGetUniformLocation(shader->programId(), "modelMatrix");
        functions->glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(tile.modelMatrix));

        // Normal matrix of the current tile
        location = functions->glGetUniformLocation(shader->programId(), "normalMatrix");
        functions->glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(tile.getNormalMatrix()));

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

    for(unsigned int i = 0; i < terrain->tiles.size(); i++)
    {

        glm::mat4 modelViewProjectionMatrix = camera->getProjectionMatrix() * camera->getViewMatrix() *terrain->tiles.at(i).modelMatrix;
        GLuint location = functions->glGetUniformLocation(shader->programId(), "modelViewProjectionMatrix");
        functions->glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(modelViewProjectionMatrix));

        if(shadingEnabled)
        {
            location = functions->glGetUniformLocation(shader->programId(), "colorTexture");
            functions->glUniform1i(location, 1);
            functions->glActiveTexture(GL_TEXTURE0 + 1);
            functions->glBindTexture(GL_TEXTURE_2D, terrain->grassTexture->textureId());

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

