#include "skyboxtechnique.h"
#include <QOpenGLShaderProgram>
#include "shaders.h"
#include "camera.h"
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"

SkyboxTechnique::SkyboxTechnique(QOpenGLFunctions_4_4_Core *functions) :
    functions(functions)
{
    QImage posX = QImage("Assets/skybox/skybox_left2048.png").rgbSwapped();
    QImage negX = QImage("Assets/skybox/skybox_right2048.png").rgbSwapped();
    QImage posY = QImage("Assets/skybox/skybox_top2048.png").rgbSwapped();
    QImage negY = QImage("Assets/skybox/skybox_bottom2048.png").rgbSwapped();
    QImage posZ = QImage("Assets/skybox/skybox_front2048.png").rgbSwapped();
    QImage negZ = QImage("Assets/skybox/skybox_back2048.png").rgbSwapped();

    cubeMapTexture = new QOpenGLTexture(QOpenGLTexture::TargetCubeMap);
    cubeMapTexture->create();
    cubeMapTexture->bind();
    cubeMapTexture->setSize(2048, 2048);
    cubeMapTexture->setFormat(QOpenGLTexture::RGBAFormat);
    cubeMapTexture->allocateStorage();
    cubeMapTexture->setMinMagFilters(QOpenGLTexture::Linear, QOpenGLTexture::Linear);
    cubeMapTexture->setWrapMode(QOpenGLTexture::ClampToEdge);

    cubeMapTexture->setData(0, 0, QOpenGLTexture::CubeMapPositiveX, QOpenGLTexture::RGBA, QOpenGLTexture::UInt8, posX.bits());
    cubeMapTexture->setData(0, 0, QOpenGLTexture::CubeMapPositiveY, QOpenGLTexture::RGBA, QOpenGLTexture::UInt8, posY.bits());
    cubeMapTexture->setData(0, 0, QOpenGLTexture::CubeMapPositiveZ, QOpenGLTexture::RGBA, QOpenGLTexture::UInt8, posZ.bits());

    cubeMapTexture->setData(0, 0, QOpenGLTexture::CubeMapNegativeX, QOpenGLTexture::RGBA, QOpenGLTexture::UInt8, negX.bits());
    cubeMapTexture->setData(0, 0, QOpenGLTexture::CubeMapNegativeY, QOpenGLTexture::RGBA, QOpenGLTexture::UInt8, negY.bits());
    cubeMapTexture->setData(0, 0, QOpenGLTexture::CubeMapNegativeZ, QOpenGLTexture::RGBA, QOpenGLTexture::UInt8, negZ.bits());

    // Our vertices. Three consecutive floats give a 3D vertex; Three consecutive vertices give a triangle.
    // A cube has 6 faces with 2 triangles each, so this makes 6*2=12 triangles, and 12*3 vertices
    float vertices[] = {
        -1.0f,-1.0f,-1.0f, // triangle 1 : begin
        -1.0f,-1.0f, 1.0f,
        -1.0f, 1.0f, 1.0f, // triangle 1 : end
        1.0f, 1.0f,-1.0f, // triangle 2 : begin
        -1.0f,-1.0f,-1.0f,
        -1.0f, 1.0f,-1.0f, // triangle 2 : end
        1.0f,-1.0f, 1.0f,
        -1.0f,-1.0f,-1.0f,
        1.0f,-1.0f,-1.0f,
        1.0f, 1.0f,-1.0f,
        1.0f,-1.0f,-1.0f,
        -1.0f,-1.0f,-1.0f,
        -1.0f,-1.0f,-1.0f,
        -1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f,-1.0f,
        1.0f,-1.0f, 1.0f,
        -1.0f,-1.0f, 1.0f,
        -1.0f,-1.0f,-1.0f,
        -1.0f, 1.0f, 1.0f,
        -1.0f,-1.0f, 1.0f,
        1.0f,-1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f,-1.0f,-1.0f,
        1.0f, 1.0f,-1.0f,
        1.0f,-1.0f,-1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f,-1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f,-1.0f,
        -1.0f, 1.0f,-1.0f,
        1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f,-1.0f,
        -1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f, 1.0f,
        1.0f,-1.0f, 1.0f
    };

    functions->glGenVertexArrays(1, &cubeVAO);
    functions->glBindVertexArray(cubeVAO);

    functions->glGenBuffers(1, &cubeVBO);
    functions->glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    functions->glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    functions->glEnableVertexAttribArray(0);
    functions->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

    functions->glBindVertexArray(0);
}

SkyboxTechnique::~SkyboxTechnique()
{
    delete cubeMapTexture;
    functions->glDeleteBuffers(1, &cubeVBO);
    functions->glDeleteVertexArrays(1, &cubeVAO);
}

void SkyboxTechnique::draw()
{
    Camera* camera = Camera::Main;
    functions->glDisable(GL_CULL_FACE);
    QOpenGLShaderProgram* shader = Shaders::Find("skybox");
    shader->bind();

    GLuint location = functions->glGetUniformLocation(shader->programId(), "cubeMap");
    functions->glUniform1i(location, 0);
    cubeMapTexture->bind(0);

    glm::mat4 modelMatrix = glm::translate(glm::mat4(1), camera->getPosition());
    glm::mat4 mvpMatrix = camera->getProjectionMatrix() * camera->getViewMatrix() * modelMatrix;

    location =  functions->glGetUniformLocation(shader->programId(), "modelViewProjectionMatrix");
    functions->glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(mvpMatrix));

    functions->glBindVertexArray(cubeVAO);
    functions->glDrawArrays(GL_TRIANGLES, 0, 12 * 3);
    functions->glBindVertexArray(0);
    shader->release();
}
