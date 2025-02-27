#include "driverBackground.h"
#include "../../geometries/boxGeometry.h"
#include "../../material/cubeMaterial.h"
#include "../../camera/camera.h"
#include "../renderer.h"

namespace gx {

    DriverBackground::DriverBackground(gx::Renderer *renderer, const DriverObjects::Ptr &objects) noexcept {
        mRenderer = renderer;
        mObjects = objects;
    }

    DriverBackground::~DriverBackground() noexcept {}

    void DriverBackground::render(const DriverRenderList::Ptr &renderList, const Scene::Ptr &scene) {
        // clear renderer
        if (mRenderer->mAutoColor) {
            mRenderer->clear();
        }

        // prepare background
        // 将CubeTexture取出，赋值给background
        auto background = scene->mBackground;
        if (background == nullptr) {
            return;
        }

        // 构造用于天空盒的立方体Mesh
        if (mBoxMesh == nullptr) {
            auto geometry = Box
        }
    }
}