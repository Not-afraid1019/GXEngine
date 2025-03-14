#pragma once
#include "camera.h"

namespace gx {
    class PerspectiveCamera : public Camera {
    public:
        using Ptr = std::shared_ptr<PerspectiveCamera>;
        static Ptr create(float near, float far, float aspect, float fov) {
            return std::make_shared<PerspectiveCamera>(near, far, aspect, fov);
        }

        PerspectiveCamera(float near, float far, float aspect, float fov) noexcept;

        ~PerspectiveCamera() noexcept;

    private:
        glm::mat4 updateProjectionMatrix() noexcept override;

    private:

        float mFar{0.0f};
        float mNear{0.0f};
        float mAspect{0.0f};
        float mFov{0.0f};
    };
}