#include "meshBasicMaterial.h"

namespace gx {

    MeshBasicMaterial::MeshBasicMaterial() noexcept {
        mType = MaterialName::MeshBasicMaterial;
        mIsMeshBasicMaterial = true;
        mDrawMode = DrawMode::Triangles;
    }

    MeshBasicMaterial::~MeshBasicMaterial() noexcept {}
}