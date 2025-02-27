#include "mesh.h"

namespace gx {

    Mesh::Mesh(const Geometry::Ptr &geometry, const Material::Ptr &material) noexcept
        : RenderableObject(geometry, material) {
        mIsMesh = true;
    }

    Mesh::~Mesh() noexcept {}
}