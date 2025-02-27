#pragma once
#include "../global/base.h"
#include "../camera/camera.h"
#include "../core/object3D.h"
#include "../objects/mesh.h"
#include "../scene/scene.h"
#include "driver/driverWindow.h"
#include "renderTarget.h"
#include "driver/driverRenderList.h"
#include "driver/driverTextures.h"
#include "driver/driverAttributes.h"
#include "driver/driverState.h"
#include "driver/driverObjects.h"
#include "driver/driverGeometries.h"
#include "driver/driverPrograms.h"
#include "driver/driverBindingState.h"
#include "driver/driverMaterials.h"
#include "driver/driverBackground.h"
#include "driver/driverRenderState.h"
#include "driver/driverRenderTargets.h"
#include "driver/driverShadowMap.h"
#include "../math/frustum.h"



namespace gx {
    
    class Renderer : public std::enable_shared_from_this<Renderer> {
    public:
        friend class DriverShadowMap;
        
        struct Descriptor {
            uint32_t mWidth{800};
            uint32_t mHeight{600};
        };
        
        using OnSizeCallback = std::function<void(int width, int height)>;

        using Ptr = std::shared_ptr<Renderer>;
        static Ptr create(const Descriptor& descriptor) {
            return std::make_shared<Renderer>(descriptor);
        }

        Renderer(const Descriptor& descriptor) noexcept;

        ~Renderer() noexcept;

        bool render(Scene::Ptr scene, Camera::Ptr camera);

        void swap() noexcept;

        void setSize(int width, int height) noexcept;

        void setRenderTarget(const RenderTarget::Ptr& renderTarget) noexcept;

        void setFrameSizeCallback(const OnSizeCallback& callback) noexcept;

//        void setMouseMoveCallback(const DriverWindow::MouseMoveCallback& callback) noexcept;

//        void setMouseActionCallback(const DriverWindow::MouseActionCallback& callback) noexcept;

//        void setKeyboardActionCallback(const DriverWindow::KeyboardActionCallback& callback) noexcept;

        RenderTarget::Ptr getRenderTarget() const noexcept;

        void setClearColor(float r, float g, float b, float a) noexcept;

        glm::vec4 getClearColor() const noexcept;

        void enableShadow(bool enable) noexcept;

        void clear(bool color=true, bool depth=true, bool stencil=true) noexcept;

    public:
        bool mAutoColor{true};

    private:
        void projectObject(const Object3D::Ptr& object, uint32_t groupOrder, bool sortObjects) noexcept;

//        void renderScene(const DriverRenderList::Ptr& currentRenderList, const Scene::Ptr& scene, const Camera::Ptr& camera) noexcept;

        void renderObjects(
//                const std::vector<RenderItem::Ptr>& renderItems,
                const Scene::Ptr& scene,
                const Camera::Ptr& camera) noexcept;

        void renderObject(
                const RenderableObject::Ptr& object,
                const Scene::Ptr& scene,
                const Camera::Ptr& camera,
                const Geometry::Ptr& geometry,
                const Material::Ptr& material) noexcept;

        void renderBufferDirect(
                const RenderableObject::Ptr& object,
                const Scene::Ptr& scene,
                const Camera::Ptr& camera,
                const Geometry::Ptr& geometry,
                const Material::Ptr& material) noexcept;

        DriverProgram::Ptr setProgram(
                const Camera::Ptr& camera,
                const Scene::Ptr& scene,
                const Geometry::Ptr& geometry,
                const Material::Ptr& material,
                const RenderableObject::Ptr& object) noexcept;

        DriverProgram::Ptr setProgram(
                const Material::Ptr& material,
                const Scene::Ptr& scene,
                const RenderableObject::Ptr& object) noexcept;

        void updateCommonMaterialProperties(
                const Material::Ptr& material,
                const DriverProgram::Parameters::Ptr& parameters) noexcept;

        bool materialNeedsLights(const Material::Ptr& material) noexcept;

        void makeLightsNeedUpdate(UniformHandleMap& lightsUniformMap) noexcept;

    private:
        int mWidth{800};
        int mHeight{600};

        OnSizeCallback mOnSizeCallback{nullptr};

        bool mSortObject{true};

        glm::mat4 mCurrentViewMatrix = glm::mat4(1.0f);

        glm::vec4 mViewport{};

        RenderTarget::Ptr mCurrentRenderTarget{nullptr};

        DriverWindow::Ptr mWindow{nullptr};
        DriverRenderList::Ptr mRenderList{nullptr};
        DriverTextures::Ptr mTextures{nullptr};
        DriverAttributes::Ptr mAttributes{nullptr};
        DriverInfo::Ptr mInfos{nullptr};
        DriverState::Ptr mState{nullptr};
        DriverObjects::Ptr mObjects{nullptr};
        DriverGeometries::Ptr mGeometries{nullptr};
        DriverPrograms::Ptr mPrograms{nullptr};
        DriverBindingStates::Ptr mBindingStates{nullptr};
        DriverMaterials::Ptr mMaterials{nullptr};
        DriverBackground::Ptr mBackground{nullptr};
        DriverRenderState::Ptr mRenderState{nullptr};
        DriverRenderTargets::Ptr mRenderTargets{nullptr};
        DriverShadowMap::Ptr mShadowMap{nullptr};

        Frustum::Ptr mFrustum{nullptr};

        // dummy objects
        Scene::Ptr mDummyScene = Scene::create();
    };
}