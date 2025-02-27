#include "depthMaterial.h"

namespace gx {

    DepthMaterial::DepthMaterial(uint32_t packing) noexcept {
        mType = MaterialName::DepthMaterial;
        mIsDepthMaterial = true;
        mDrawMode = DrawMode::Triangles;
    }

    DepthMaterial::~DepthMaterial() noexcept {}
}