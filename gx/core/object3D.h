#pragma once
#include "../global/base.h"

namespace gx {

    class ObjectTypeChecker {
    public:
        bool mIsRenderableObject{false};
        bool mIsMesh{false};
        bool mIsSkinnedMesh{false};
        bool mIsBone{false};
        bool mIsScene{false};
        bool mIsCamera{false};
        bool mIsPerspectiveCamera{false};
        bool mIsOrthographicCamera{false};
        bool mIsGroup{false};
        bool mIsLight{false};
        bool mIsAmbientLight{false};
        bool mIsDirectionalLight{false};
    };

    // Object3D是所有空间变换、节点结构等的最基础的类型
    class Object3D : public std::enable_shared_from_this<Object3D>, public ObjectTypeChecker {
    public:
        // 使用智能指针进行管理
        using Ptr = std::shared_ptr<Object3D>;
        static Ptr create() {
            return std::make_shared<Object3D>();
        }

        Object3D() noexcept;

        ~Object3D() noexcept;

        // 满足动态类型转换,dynamic_cast要求父类必要含有虚函数
        // 智能指针也拥有一套转换体系
        virtual void fakeFunction() noexcept {}

        void setPosition(float x, float y, float z) noexcept;

        void setPosition(const glm::vec3& position) noexcept;

        void setQuaternion(float x, float y, float z, float w) noexcept;

        void setScale(float x, float y, float z) noexcept;

        void rotateX(float angle) noexcept;

        void rotateY(float angle) noexcept;

        void rotateZ(float angle) noexcept;

        void setRotateAroundAxis(const glm::vec3& axis, float angle) noexcept;

        void rotateAroundAxis(const glm::vec3& axis, float angle) noexcept;

        void lookAt(const glm::vec3& target, const glm::vec3& up) noexcept;

        void setLocalMatrix(const glm::mat4& localMatrix) noexcept;

        void setWorldMatrix(const glm::mat4& worldMatrix) noexcept;

        // 向当前的Object3D里面,加入子节点
        void addChild(const Object3D::Ptr& child) noexcept;

        virtual void updateMatrix() noexcept;

        virtual glm::mat4 updateWorldMatrix(bool updateParent = false, bool updateChildren = false) noexcept;

        glm::mat4 updateModelViewMatrix(const glm::mat4& viewMatrix) noexcept;

        glm::mat3 updateNormalMatrix() noexcept;

        glm::vec3 getPosition() const noexcept;

        glm::vec3 getWorldPosition() const noexcept;

        glm::vec3 getLocalDirection() const noexcept;

        glm::vec3 getWorldDirection() const noexcept;

        glm::vec3 getUp() const noexcept;

        glm::vec3 getRight() const noexcept;

        glm::mat4 getLocalMatrix() noexcept;

        glm::mat4 getWorldMatrix() noexcept;

        glm::mat4 getModelViewMatrix() noexcept;

        glm::mat4 getNormalMatrix() noexcept;

        const std::vector<Object3D::Ptr>& getChildren() const noexcept;

        ID getID() const noexcept;

    protected:
        void decompose() noexcept;

    public:
        // visible来表示是否对其进行渲染
        bool mVisible{true};

        // 是否产生阴影
        bool mCastShadow{true};

        // obj的名字
        std::string mName;

        // 表示是否强制对矩阵进行更新
        bool mNeedsUpdateMatrix{true};

//    protected:
        ID mID{0};  // 全局唯一id

        // for setting easy
        glm::vec3 mPosition{glm::vec3(0.0f)};

        // 对于object的旋转变换,一律采用四元数
        glm::quat mQuaternion{glm::quat(1.0f, 0.0f, 0.0f, 0.0f)};

        // x y z放缩倍数
        glm::vec3 mScale{glm::vec3(1.0f)};

        // localMatrix对其模型坐标系进行变换
        glm::mat4 mLocalMatrix = glm::mat4(1.0f);

        // worldMatrix将模型顶点从模型坐标系转换到世界坐标系
        glm::mat4 mWorldMatrix = glm::mat4(1.0f);

        // 保留参数
        bool mNeedsUpdate{false};

        // 节点系统,
        // 父节点采用weak_ptr,防止循环引用
        std::weak_ptr<Object3D> mParent;
        // 父节点存储了子节点的shared_ptr,建立一次引用,保证子节点的引用计数至少大于1
        std::vector<Object3D::Ptr> mChildren{};

        // for shading
        // modelViewMatrix将模型顶点从模型坐标系转换到当前摄像机坐标系,viewMatrix * worldMatrix
        glm::mat4 mModelViewMatrix = glm::mat4(1.0f);

        // 将模型的normal从模型坐标系转换到摄像机坐标系
        glm::mat3 mNormalMatrix = glm::mat3(1.0f);
    };
}
