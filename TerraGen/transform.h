#pragma once
#include <vector>

#define GLM_FORCE_RADIANS
#include "glm\gtc\matrix_transform.hpp"
#include "glm\glm.hpp"


class Transform
{
public:
    Transform();

    ~Transform();

    void update(float deltaT);

    void rotate(glm::vec3 axis, float angle);

    void rotateWorld(glm::vec3 axis, float angle);

    void translate(glm::vec3 translation);

    void scale(glm::vec3 scale);

    void setPosition(glm::vec3 position);

    void setRotation(glm::vec3 eulerAngles);

    void setScale(glm::vec3 scale);

    glm::vec3 getPosition();

    glm::vec3 getRotation();

    glm::mat3 getNormalMatrix();

    glm::mat4 getInverseModelMatrix();

    glm::mat4 modelMatrix;

    // Todo add Material??
};

