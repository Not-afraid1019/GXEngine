#pragma once
#include "../global/base.h"
#include "../core/object3D.h"

namespace gx {

    // 在使用Assimp读取模型的时候，每一个aiNode都会形成一个Bone
    //
    class Bone : public Object3D {
    public:
        using Ptr = std::shared_ptr<Bone>;
        static Ptr create() {
            return std::make_shared<Bone>();
        }

        Bone() noexcept;

        ~Bone() noexcept;

        glm::mat4 updateWorldMatrix(bool updateParent = false, bool updateChildren = false) noexcept override;

    public:
        // 对应着Node的mTransform
        glm::mat4 mNodeMatrix = glm::mat4(1.0f);
//        glm::mat4 mBoneMatrix = glm::mat4(1.0f);
//        glm::mat4 mOffsetMatrix = glm::mat4(1.0f);
    };
}
