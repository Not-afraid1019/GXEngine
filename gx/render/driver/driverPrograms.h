#pragma once
#include "../../global/base.h"
#include "../../objects/renderableObject.h"
#include "../../material/material.h"
#include "driverUniforms.h"
#include "driverLights.h"
#include "driverShadowMap.h"


namespace gx {

    class DriverPrograms;
    class DriverProgram {
        friend DriverPrograms;
    public:

        struct Parameters {
            using Ptr = std::shared_ptr<Parameters>;
            static Ptr create() { return std::make_shared<Parameters>(); }

            std::string mShaderID;  // material的typeName
            std::string mVertex;    // vs的代码
            std::string mFragment;  // fs的代码

            bool mInstancing{false};    // 是否启用实例绘制,TODO
            bool mHasNormal{false};     // 本次绘制的模型是否有法线
            bool mHasUV{false};         // 本次绘制的模型是否有uv
            bool mHasColor{false};      // 本次绘制的模型是否有顶点颜色
            bool mHasDiffuseMap{false}; // 本次绘制的模型是否有diffuseMap
            bool mHasEnvCubeMap{false}; // 本次绘制的模型是否有环境贴图
            bool mHasSpecularMap{false};// 本次绘制的模型是否有镜面反射贴图

            bool mShadowMapEnabled{false};  // 是否启用阴影
            uint32_t mDirectionalLightCount{0};
            uint32_t mNumDirectionalLightShadow{0};

            bool mUseTangent{false};
            bool mUseNormalMap{false};

            bool mSkinning{false};
            uint32_t mMaxBones{0};

            uint32_t mDepthPacking{0};
        };

        using Ptr = std::shared_ptr<DriverProgram>;
        static Ptr create(const Parameters::Ptr& parameters) {
            return std::make_shared<DriverProgram>(parameters);
        }

        DriverProgram(const Parameters::Ptr& parameters) noexcept;

        ~DriverProgram() noexcept;

        auto getID() const { return mID; }

        auto getCacheKey() const noexcept { return mCacheKey; }

        GLuint mProgram{0};

        void uploadUniforms(UniformHandleMap& uniformHandleMap, const DriverTextures::Ptr& textures);

    private:
        void replaceAttributeLocation(std::string& shader) noexcept;
        void replaceLightNumbers(std::string& shader, const Parameters::Ptr& parameters) noexcept;

        std::string getExtensionString() noexcept;

    private:
        ID mID{0};  // driverProgram自己的id号
        HashType mCacheKey{0};  // 由parameters参数集合计算出来的hash值
        uint32_t mRefCount{0};  // 控制外界有多少引用本Program的renderItem
        DriverUniforms::Ptr mUniforms{nullptr};
    };

    class DriverPrograms {
    public:
        using Ptr = std::shared_ptr<DriverPrograms>;
        static Ptr create() {
            return std::make_shared<DriverPrograms>();
        }

        DriverPrograms() noexcept;

        ~DriverPrograms() noexcept;

        DriverProgram::Ptr acquireProgram(const DriverProgram::Parameters::Ptr& parameters, HashType cacheType) noexcept;

        UniformHandleMap getUniforms(const Material::Ptr& material) noexcept;

        DriverProgram::Parameters::Ptr getParameters(
                const Material::Ptr& material,
                const Object3D::Ptr& object,
                const DriverLights::Ptr& lights,
                const DriverShadowMap::Ptr& shadowMap) noexcept;

        HashType getProgramCacheKey(const DriverProgram::Parameters::Ptr& parameters) noexcept;

        void release(const DriverProgram::Ptr& program) noexcept;

    private:
        std::unordered_map<HashType, DriverProgram::Ptr> mPrograms{};
    };
}