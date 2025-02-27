#pragma once
#include "../../global/base.h"
#include "../renderTarget.h"
#include "../../global/eventDispatcher.h"

namespace gx {

    class DriverRenderTarget {
    public:
        using Ptr = std::shared_ptr<DriverRenderTarget>;
        static Ptr create() { return std::make_shared<DriverRenderTarget>(); }

        DriverRenderTarget() noexcept;

        ~DriverRenderTarget() noexcept;

        void generateFrameBuffer() noexcept;

    public:
        GLuint mFrameBuffer{0};

        // 如果用户没有在外部生成DepthTexture，说明用户不想回都渲染后的深度数据
        // 对于用户自定义的DepthTexture，管理权限归于DriverTextures
        //
        // 所以必须自己生成一个RenderBuffer，类型就是RenderBuffer类型，这个类型与Texture类型相似
        // 也可以加入到mFrameBuffer当中作为一个附件
        //
        // RenderBuffer其实就是一个数组，不用编码人格Texture的格式，在OpenGL管线当中会更加有效率
        //
        GLuint mDepthRenderBuffer{0};
    };

    class DriverRenderTargets {
    public:
        using Ptr = std::shared_ptr<DriverRenderTargets>;
        static Ptr create() { return std::make_shared<DriverRenderTargets>(); }

        DriverRenderTargets() noexcept;

        ~DriverRenderTargets() noexcept;

        DriverRenderTarget::Ptr get(const RenderTarget::Ptr& renderTarget) noexcept;

        void onRenderTargetDispose(const EventBase::Ptr& e);

    private:
        std::unordered_map<ID, DriverRenderTarget::Ptr> mRenderTargets{};
    };
}