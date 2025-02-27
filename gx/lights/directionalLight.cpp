#include "directionalLight.h"

namespace gx {

    DirectionalLight::DirectionalLight() noexcept {
        mIsDirectionalLight = true;
        mShadow = DirectionalLightShadow::create();
    }

    DirectionalLight::~DirectionalLight() noexcept {}
}