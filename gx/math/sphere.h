#pragma once

#include "../global/base.h"

namespace gx {

    class Sphere {
    public:
        // 只智能指针包装
        using Ptr = std::shared_ptr<Sphere>;
        static Ptr create(const glm::vec3& center, float radius) {
            return std::make_shared<Sphere>(center, radius);
        }

        Sphere(const glm::vec3& center, float radius) noexcept {
            mCenter = center;
            mRadius = radius;
        }

        ~Sphere() noexcept {}

        // 应用在跟随物体进行matrix变换的时候
        void applyMatrix4(const glm::mat4 matrix) noexcept {
            mCenter = glm::vec3(matrix * glm::vec4(mCenter, 1.0f));

            float scaleX = glm::length(glm::vec3(matrix[0]));
            float scaleY = glm::length(glm::vec3(matrix[1]));
            float scaleZ = glm::length(glm::vec3(matrix[2]));

            float maxScale = std::max(std::max(scaleX, scaleY), scaleZ);
            mRadius *= maxScale;
        }

        void copy(const Sphere::Ptr& other) {
            mCenter = other->mCenter;
            mRadius = other->mRadius;
        }

    public:
        glm::vec3 mCenter = glm::vec3(0.0f);
        float mRadius{0.0f};
    };
}