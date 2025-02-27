#pragma once
#include "../global/base.h"
#include "../global/constant.h"
#include "../core/object3D.h"
#include "lightShadow.h"

namespace gx {

    class Light : public Object3D {
    public:
        using Ptr = std::shared_ptr<Light>;
        static Ptr create() {
            return std::make_shared<Light>();
        }

        Light() noexcept;

        ~Light() noexcept;

    public:
        glm::vec3 mColor = glm::vec3(1.0f);
        float mIntensity = 1.0f;
        bool mCastShadow{false};
        LightShadow::Ptr mShadow{nullptr};
    };
}