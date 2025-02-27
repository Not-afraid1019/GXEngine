#pragma once
#include "light.h"
#include "directionalLightShadow.h"

namespace gx {

    class DirectionalLight : public Light {
    public:
        using Ptr = std::shared_ptr<DirectionalLight>;
        static Ptr create() {
            return std::make_shared<DirectionalLight>();
        }

        DirectionalLight() noexcept;

        ~DirectionalLight() noexcept;
    };
}