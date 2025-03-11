#include "cubeMaterial.h"

namespace gx {

    CubeMaterial::CubeMaterial() noexcept {
        mType = MaterialName::Material;
        mIsCubeMaterial = true;
        mDrawMode = DrawMode::Triangles;
    }

    CubeMaterial::~CubeMaterial() noexcept {}
}