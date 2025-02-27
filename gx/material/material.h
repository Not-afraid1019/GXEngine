#pragma once
#include "../global/base.h"
#include "../global/constant.h"
#include "../textures/texture.h"
#include "../textures/cubeTexture.h"

namespace gx {
    // 材质类，描述一个mesh绘制的时候，用什么贴图，走什么样的管线，过什么样的shader
    // 根据材质类型不同，可能会有一些特殊参数
    // 多个mesh可以应用同一个材质Material Ptr
    class MaterialTypeChecker {
    public:
        bool mIsMaterial = false;
        bool mIsMeshBasicMaterial = false;
        bool mIsMeshPhongMaterial = false;
        bool mIsLineBasicMaterial = false;
        bool mIsCubeMaterial = false;
        bool mIsDepthMaterial = false;
    };

    class Material : public MaterialTypeChecker {
    public:
        // 使用智能指针管理
        using Ptr = std::shared_ptr<Material>;
        static Ptr create() {
            return std::make_shared<Material>();
        }

        Material() noexcept;

        ~Material() noexcept;

    public:
        std::string getType() const noexcept {}
        ID getID() const noexcept {}

        // 表示在backend里面，是否需要更新材质参数
        bool mNeedsUpdate{true};

        // version 用于首次解析
        uint32_t mVersion{1};

        // raster
        FrontFace mFrontFace{FrontFace::FrontCounterClockWise};
        Side mSide{Side::DoubleSide};

        DrawMode mDrawMode{DrawMode::Triangles};

        // blending
        bool mTransparent{false};
        float mOpacity{1.0f};
        BlendingType mBlendingType{BlendingType::CustomBlending};
        BlendingFactor mBlendSrc{BlendingFactor::SrcAlpha};
        BlendingFactor mBlendDst{BlendingFactor::OneMinusSrcAlpha};
        BlendingEquation mBlendingEquation{BlendingEquation::AddEquation};
        BlendingFactor mBlendSrcAlpha{BlendingFactor::None};
        BlendingFactor mBlendDstAlpha{BlendingFactor::Zero};
        BlendingEquation mBlendingEquationAlpha{BlendingEquation::AddEquation};

        // depth
        bool mDepthTest{true};
        bool mDepthWrite{true};
        CompareFunction mDepthFunction{CompareFunction::LessOrEqual};
        double mDepthClearColor{1.0};

        // diffuse
        Texture::Ptr mDiffuseMap{nullptr};

        // envMap
        CubeTexture::Ptr mEnvMap{nullptr};

        // normalMap
        Texture::Ptr mNormalMap{nullptr};

        // specularMap
        Texture::Ptr mSpecularMap{nullptr};

    protected:
        ID mID{0};
        std::string mType;
    };
}
