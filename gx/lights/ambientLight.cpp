#include "ambientLight.h"

namespace gx {
    AmbientLight::AmbientLight() noexcept {
        mIsAmbientLight = true;
        mIntensity = 0.1f;
    }

    AmbientLight::~AmbientLight() noexcept {}
}