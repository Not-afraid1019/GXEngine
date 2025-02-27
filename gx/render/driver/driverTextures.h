#pragma once
#include "../../global/base.h"
#include "../../textures/texture.h"
#include "../../textures/cubeTexture.h"
#include "../../global/eventDispatcher.h"
#include "driverInfo.h"
#include "../renderTarget.h"
#include "driverRenderTargets.h"

namespace gx {

    class DriverTexture {
    public:
        using Ptr = std::shared_ptr<DriverTexture>;
        static Ptr create() {
            return std::make_shared<DriverTexture>();
        }

        DriverTexture() noexcept;

        ~DriverTexture() noexcept;

        void dispose() noexcept;

        // 通过glGenTextures获得的texture编号
        GLuint mHandle{0};

    };

    class DriverTextures {
    public:
        using Ptr = std::shared_ptr<DriverTextures>;
        static Ptr create(const DriverInfo::Ptr& info, const DriverRenderTargets::Ptr& renderTargets) {
            return std::make_shared<DriverTextures>(info, renderTargets);
        }

        DriverTextures(const DriverInfo::Ptr& info, const DriverRenderTargets::Ptr& renderTargets) noexcept;

        ~DriverTextures() noexcept;

        // 传入texture，获得其对应的DriverTexture
        DriverTexture::Ptr get(const Texture::Ptr& texture) noexcept;

        //
        // 将texture对应的DriverTexture中的Handle绑定到textureUnit代表的Slot上
        // GL_TEXTURE0 GL_TEXTURE1
        void bindTexture(const Texture::Ptr& texture, GLenum textureUnit);

        void setupRenderTarget(const RenderTarget::Ptr& renderTarget) noexcept;

        void onTextureDestroy(const EventBase::Ptr& e) noexcept;

    private:
        // 要么新建一个texture，要么跟原有texture的属性数据或者内容数据
        void update(const Texture::Ptr& texture) noexcept;

        DriverTexture::Ptr setupDriverTexture(const Texture::Ptr& texture) noexcept;

        void setupFBOColorAttachment(const GLuint& fbo, const GLenum& attachment, const Texture::Ptr& texture) noexcept;

        void setupFBODepthStencilAttachment(const RenderTarget::Ptr& renderTarget) noexcept;

        void setupDepthTexture(const GLuint& frameBuffer, const RenderTarget::Ptr& renderTarget) noexcept;

        void setupDepthRenderBuffer(const GLuint& frameBuffer, const RenderTarget::Ptr& renderTarget) noexcept;

    private:
        DriverInfo::Ptr mInfo{nullptr};
        DriverRenderTargets::Ptr mRenderTargets{nullptr};
        std::unordered_map<ID, DriverTexture::Ptr> mTextures{};
    };
}