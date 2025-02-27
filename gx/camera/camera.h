#pragma once
#include "../global/base.h"
#include "../core/object3D.h"

namespace gx {

    // worldMatrix将顶点从模型坐标系转换到世界坐标系
    // viewMatrix将顶点坐标系转换到模型坐标系
    // worldMatrix * viewMatrix = I(单位矩阵)
    // Camera：一是作为游戏主视角，二是作为光照主视角
    class Camera : public Object3D {
    public:
        using Ptr = std::shared_ptr<Camera>;

        Camera() noexcept;

        ~Camera() noexcept;

        glm::mat4 updateWorldMatrix(bool updateParent = false, bool updateChildren = false) noexcept override;

        glm::mat4 getWorldMatrixInverse() const noexcept { return mWorldMatrixInverse; }

        glm::mat4 getProjectionMatrix() const noexcept { return mProjectionMatrix; }

        virtual glm::mat4 updateProjectionMatrix() noexcept = 0;

    protected:
        glm::mat4 mWorldMatrixInverse = glm::mat4(1.0f);
        glm::mat4 mProjectionMatrix = glm::mat4(1.0f);
    };
}
