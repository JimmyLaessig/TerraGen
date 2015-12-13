#include "noisetexturegenerator.h"

NoiseTextureGenerator::NoiseTextureGenerator()
{

}

QImage* NoiseTextureGenerator::Generate(QOpenGLFunctions_4_4_Core *functions, int width, int height)
{
    // Create Framebuffer to render the noise texture to
    QOpenGLFramebufferObject* framebuffer = new QOpenGLFramebufferObject(width, height);

    // Create Quad for single renderin
    GLfloat quadVertices[] = {
        // Positions
        -1.0f,  1.0f, 0.0f,
        -1.0f, -1.0f, 0.0f,
        1.0f,  1.0f, 0.0f,
        1.0f, -1.0f, 0.0f
    };
    // Setup VAO
    GLuint VAO;

    functions->glGenVertexArrays(1, &VAO);
    functions->glBindVertexArray(VAO);

    GLuint positionVBO;
    functions->glGenBuffers(1, &positionVBO);

    functions->glBindBuffer(GL_ARRAY_BUFFER, positionVBO);
    functions->glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
    functions->glEnableVertexAttribArray(0);
    functions->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

    // Bind Shader
    QOpenGLShaderProgram* shader = Shaders::Find("perlinNoise");
    shader->bind();

    // Bind Framebuffer and set Viewport
    framebuffer->bind();
    functions->glViewport(0,0,width, height);

    // Draw Noise to framebuffer
    functions->glBindVertexArray(VAO);
    functions->glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    functions->glBindVertexArray(0);

    // Release Shader and Framebuffer
    shader->release();
    framebuffer->release();

    // Delete Quad
    functions-> glDeleteVertexArrays(1,&positionVBO );
    functions->glDeleteBuffers(1, &VAO);

    // Retrieve Image from framebuffer
    QImage* image = new QImage(framebuffer->toImage());
    delete framebuffer;

    return image;
}
