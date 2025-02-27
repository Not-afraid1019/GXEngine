#include "renderableObject.h"

namespace gx {

    RenderableObject::RenderableObject(const Geometry::Ptr &geometry, const Material::Ptr &material) noexcept {
        mGeometry = geometry;
        mMaterial = material;

        mIsRenderableObject = true;
    }

    RenderableObject::~RenderableObject() noexcept {}

    void RenderableObject::onBeforeRender(gx::Renderer *renderer, gx::Scene *scene, gx::Camera *camera) {
        if (mOnBeforeRenderCallback) {
            mOnBeforeRenderCallback(renderer, scene, camera);
        }
    }
}