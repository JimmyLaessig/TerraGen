#include "shadowmaptechnique.h"
#include "camera.h"
#include <QOpenGLShaderProgram>
#include "shaders.h"
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"

ShadowMapTechnique::ShadowMapTechnique(QOpenGLFunctions_4_4_Core *functions, int width, int height):
    functions(functions),
    width(width),
    height(height)
{
    // Create Framebuffer
    functions->glGenFramebuffers(1, &fbo);

    // Create Depth Texture
    functions->glGenTextures(1, &depthTexture);
    functions->glBindTexture(GL_TEXTURE_2D, depthTexture);

    functions->glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, width, height, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, 0);

    functions->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    functions->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    functions->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    functions->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    functions->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE,GL_COMPARE_REF_TO_TEXTURE);
    functions->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);

    functions->glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    functions->glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthTexture, 0);

    // Disable writing to the color buffer
    functions->glDrawBuffer(GL_NONE);
    functions->glReadBuffer(GL_NONE);

   GLenum Status =  functions->glCheckFramebufferStatus(GL_FRAMEBUFFER);

    if (Status != GL_FRAMEBUFFER_COMPLETE) {
    }

    functions->glBindFramebuffer(GL_FRAMEBUFFER, 0);

}
void ShadowMapTechnique::bindShadowTexture(int unit)
{
    functions->glActiveTexture(GL_TEXTURE0 + unit);
    functions->glBindTexture(GL_TEXTURE_2D, depthTexture);
}

void ShadowMapTechnique::drawShadowMap(Terrain *terrain)
{
    Camera* camera = Camera::Main;

    functions->glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    functions->glViewport(0, 0, width, height);

    // functions->glCullFace(GL_FRONT);
    functions->glPolygonMode(GL_FRONT, GL_FILL);

    //functions->glEnable(GL_POLYGON_OFFSET_FILL);
   // functions->glPolygonOffset(4.f, 0.f);


    functions->glClearDepth(1.0);
    functions->glClear(GL_DEPTH_BUFFER_BIT);

    QOpenGLShaderProgram* shader = Shaders::Find("shadowMap");

    shader->bind();

    int unit = 1;
    // Max height of the terrain
    GLuint location =  functions->glGetUniformLocation(shader->programId(), "maxHeight");
    functions->glUniform1f(location, terrain->maxHeight);

    // Heightmap texture
    location =  functions->glGetUniformLocation(shader->programId(), "heightmapTexture");
    functions->glUniform1i(location, unit);
    terrain->heightmapTexture->bind(unit++);

    // Position of the camera in world space
    location =  functions->glGetUniformLocation(shader->programId(), "eyePosWorld");
    functions->glUniform3fv(location, 1, glm::value_ptr(camera->getPosition()));

    // Number of tiles in the grid
    location =  functions->glGetUniformLocation(shader->programId(), "numTiles");
    functions->glUniform2f(location, terrain->getGridRepetitionX(), terrain->getGridRepetitionY());

    // Direction of the light in world space
    location =  functions->glGetUniformLocation(shader->programId(), "lightDirection_World");
    functions->glUniform3fv(location, 1, glm::value_ptr(light->direction));

    // ViewProjectionMatrix of the camera
   glm::mat4 viewProjectionMatrix = light->getProjectionMatrix() * light->getViewMatrix();

    // Render all tiles
    for (unsigned int i = 0; i < terrain->tiles.size(); i++)
    {
        Transform tile = terrain->tiles.at(i);

        // Coordinates of the current tile in the grid
        location =  functions->glGetUniformLocation(shader->programId(), "tileIndex");
        functions->glUniform2fv(location,1, glm::value_ptr(terrain->tileCoordinates.at(i)));

        // MVP matrix of the current tile
        location =  functions->glGetUniformLocation(shader->programId(), "depthMVPMatrix");
        functions->glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(viewProjectionMatrix * tile.modelMatrix));

        // Model matrix of the current tile
        location =  functions->glGetUniformLocation(shader->programId(), "modelMatrix");
        functions->glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(tile.modelMatrix));

        terrain->drawTesselate();
    }
    shader->release();

    functions->glBindFramebuffer(GL_FRAMEBUFFER, 0);
    functions->glDisable(GL_POLYGON_OFFSET_FILL);
}


