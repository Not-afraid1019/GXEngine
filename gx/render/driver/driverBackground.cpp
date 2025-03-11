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
            // 构建一个1*1*1的boxGeometry
            auto geometry = BoxGeometry::create(1.0f);
            geometry->deleteAttribute("normal");
            geometry->deleteAttribute("uv");

            // 摄像机永远放在box的内部中心，所以只绘制内部即可
            auto material = CubeMaterial::create();
            material->mSide = Side::BackSide;
            material->mEnvMap = background;

            mBoxMesh = Mesh::create(geometry, material);

            // 当前的mBoxMesh仍处于世界坐标系的（0，0，0）位置，这个box需要跟着Camera移动
            // 在这个mBoxMesh进行绘制之前，做这个变换操作
            mBoxMesh->mOnBeforeRenderCallback = [&](Renderer* render, Scene* scene, Camera* camera) {
                mBoxMesh->setPosition(camera->getWorldPosition());
                mBoxMesh->updateWorldMatrix();
            };

            // 在这里要单独对这个mesh进行一次解析，创建其VBO等
            mObjects->update(mBoxMesh);
        }

        renderList->push(mBoxMesh, mBoxMesh->getGeometry(), mBoxMesh->getMaterial(), 0, 0);
    }
}