#pragma once
#include "Vector2.h"
#include "Core/Serializable.h"


#define GLM_ENABLE_EXPERIMENTAL
#include "../glm/gtx/euler_angles.hpp"
#include "../glm/gtc/matrix_transform.hpp"
#include "../glm/glm.hpp"

namespace neu {
    struct Transform : public ISerializable {
        glm::vec3 position{ 0.0f, 0.0f, 0.0f };
        glm::vec3 rotation{ 0.0f, 0.0f, 0.0f }; // in degrees
        glm::vec3 scale{ 1.0f, 1.0f, 1.0f };

        Transform() = default;

        Transform(const glm::vec3& position,
            const glm::vec3& rotation = { 0.0f, 0.0f, 0.0f },
            const glm::vec3& scale = { 1.0f, 1.0f, 1.0f })
            : position{ position }, rotation{ rotation }, scale{ scale }
        {
        }

        glm::mat4 GetMatrix() const {
            glm::mat4 mx = glm::mat4(1.0f);

            // Apply translation
            mx = glm::translate(mx, position);

            // Apply rotation (YXZ order)
            glm::mat4 rot = glm::eulerAngleYXZ(
                glm::radians(rotation.y),
                glm::radians(rotation.x),
                glm::radians(rotation.z)
            );
            mx *= rot;

            // Apply scale
            mx = glm::scale(mx, scale);

            return mx;
        }

        void Read(const serial_data_t& value) override;
    };
}