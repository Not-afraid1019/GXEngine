#pragma once
#include "camera.h"

namespace gx {

    class OrthographicCamara : public Camera {
    public:
        using Ptr = std::shared_ptr<OrthographicCamara>;
        static Ptr create(float left, float right, float bottom, float top, float near, float far) {
            return std::make_shared<OrthographicCamara>(left, right, bottom, top, near, far);
        }

        OrthographicCamara(float left, float right, float bottom, float top, float near, float far) noexcept;

        ~OrthographicCamara() noexcept;

    private:
        glm::mat4 updateProjectionMatrix() noexcept override;

    private:
        float mLeft{0.0f};
        float mRight{0.0f};
        float mBottom{0.0f};
        float mTop{0.0f};
        float mNear{0.0f};
        float mFar{0.0f};
    };
}
