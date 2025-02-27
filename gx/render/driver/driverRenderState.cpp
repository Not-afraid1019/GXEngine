#include "driverRenderState.h"

namespace gx {

    DriverRendererState::DriverRendererState() noexcept {

    }

    DriverRendererState::~DriverRendererState() noexcept {}

    void DriverRendererState::init() noexcept {
        mLightsArray.clear();
        mShadowsArray.clear();
        mLights->init();
    }

    // 更新了一些与坐标系选择无关的uniform-color
    void DriverRendererState::setupLights() noexcept {
        mLights->setupLights(mLightsArray);
    }

    // 更新了一些与坐标系选择有关的uniform-direction
    void DriverRendererState::setupLightsView(const Camera::Ptr &camera) noexcept {
        mLights->setupLightsView(mLightsArray, camera);
    }

    void DriverRendererState::pushLight(const Light::Ptr &light) noexcept {
        mLightsArray.push_back(light);
    }

    void DriverRendererState::pushShadow(const Light::Ptr &shadowLight) noexcept {
        mShadowsArray.push_back(shadowLight);
    }
}