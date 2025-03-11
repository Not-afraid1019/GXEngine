#include "directionalLightShadow.h"
#include "../camera/orthographicCamera.h"

namespace gx {

    DirectionalLightShadow::DirectionalLightShadow() noexcept :
            LightShadow(OrthographicCamara::create(-10.0f, 10.0f, -10.0f, 10.0f, -100.0f, 100.0f)) {
    }

    DirectionalLightShadow::~DirectionalLightShadow() noexcept {}
}