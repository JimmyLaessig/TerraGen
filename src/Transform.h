#pragma once
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/euler_angles.hpp>

#include "glm/gtx/transform.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/detail/type_half.hpp>


struct Transform
{
public:

    Transform() = default;


    Transform(const Transform& other);


    Transform(Transform&& other) noexcept;


    Transform& operator=(Transform&& other) noexcept;


    Transform& operator=(const Transform& other);


    glm::mat4 getLocalToWorldMatrix() const;


    glm::mat4 getWorldToLocalMatrix() const;


    glm::mat4 getLocalToWorldNormalMatrix() const;


    const glm::vec3 getPosition() const;


    const glm::vec3 getEulerAngles() const;


    const glm::quat getRotation() const;


    const glm::vec3 getScale() const;


    void setScale(const glm::vec3& scale);


    void setPosition(const glm::vec3& position);


    void setRotation(const glm::quat& rotation);


    void setRotation(const glm::vec3& eulerAngles);


    void setPositionAndRotation(const glm::vec3& position, const glm::vec3& eulerAngles);


    void setPositionAndRotation(const glm::vec3& position, const glm::quat& eulerAngles);


    void setPositionRotationAndScale(const glm::vec3& position, const glm::vec3& eulerAngles, const glm::vec3& scale);


    void setPositionRotationAndScale(const glm::vec3& position, const glm::quat& rotation, const glm::vec3& scale);


    glm::vec3 transformPosition(const glm::vec3& position);


    glm::vec3 transformVector(const glm::vec3& direction);


    glm::vec3 forward() const;

    glm::vec3 up() const;

    glm::vec3 right() const;

private:

    glm::vec3 position		= glm::vec3(0);

    glm::vec3 scale			= glm::vec3(1);

    glm::quat rotation		= glm::quat(glm::vec3(0, 0, 0));

    glm::vec3 eulerAngles	= glm::vec3(0);

    mutable bool bOutdated = false;

    mutable glm::mat4 localToWorldMatrix = glm::mat4(1);

    mutable glm::mat4 worldToLocalMatrix = glm::mat4(1);

    mutable glm::mat3 localToWorldNormalMatrix = glm::mat3(1);


    void computeMatrices() const;
};
