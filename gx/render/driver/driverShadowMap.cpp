#include "driverShadowMap.h"
#include "../renderer.h"

namespace gx {

    DriverShadowMap::DriverShadowMap(gx::Renderer *renderer, const std::shared_ptr<DriverObjects> &objects, const std::shared_ptr<DriverState> &state) noexcept {
        mRenderer = renderer;
        mObjects = objects;
        mState = state;
    }

    DriverShadowMap::~DriverShadowMap() noexcept {}

    // 任务
    // 1 填充Uniforms(shadowMapUniform, shadowMatrixUniform)
    // 2 生成每个光源的RenderTarget
    // 3 为每个光源渲染自己的ShadowMap
    //
    void DriverShadowMap::render(const std::shared_ptr<DriverRenderState> &renderState, const Scene::Ptr &scene, const Camera::Ptr &camera) noexcept {
        if (!mEnabled) return;

        // store old state
        auto currentRenderTarget = mRenderer->getRenderTarget();
        auto currentClearColor = mRenderer->getClearColor();

        mState->setBlending(BlendingType::NoBlending);
        mState->setDepth(true, true, CompareFunction::Less, 1.0);

        mRenderer->setClearColor(1.0, 1.0, 1.0, 1.0);

        // render depth map
    }
}