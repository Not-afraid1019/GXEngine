#include "driverPrograms.h"
#include "../../tools/identity.h"
#include "../shaders/shaderLib.h"
#include "../../material/depthMaterial.h"



namespace gx {

    // 1 需要对很多功能进行#define操作，从而决定打开哪些代码段
    // 2 占位字符串的替换:比如POSITION_LOCATION占位符替换为0
    DriverProgram::DriverProgram(const Parameters::Ptr &parameters) noexcept {
        mID = Identity::generateID();

        // 1 shader版本字符串
        std::string versionString = "#version 330 core\n";

        // 2 shader扩展字符串
        std::string extensionString = getExtensionString();

        // 3 prefix字符串，define的各类操作都会加入到prefix当中，从而决定后续代码当中哪些功能可以被打开
        std::string prefixVertex;
        std::string prefixFragment;

        prefixVertex.append(parameters->mHasNormal ? "#define HAS_NORMAL\n" : "");
        prefixVertex.append(parameters->mHasUV ? "#define HAS_UV\n" : "");
        prefixVertex.append(parameters->mHasColor ? "#define HAS_COLOR\n" : "");

        prefixVertex.append(parameters->mShadowMapEnabled ? "#define USE_SHADOWMAP\n" :"");
        prefixVertex.append(parameters->mSkinning ? "#define USE_SKINNING\n" : "");
        prefixVertex.append(parameters->mSkinning ? std::string("#define MAX_BONES ") + std::to_string(parameters->mMaxBones) + "\n" : "");
        prefixVertex.append(parameters->mUseNormalMap ? "#define USE_NORMALMAP\n" : "");
        prefixVertex.append(parameters->mUseTangent ? "#define USE_TANGENT\n" : "");

        prefixFragment.append(parameters->mHasNormal ? "#define HAS_NORMAL\n" : "");
        prefixFragment.append(parameters->mHasUV ? "#define HAS_UV\n" : "");
        prefixFragment.append(parameters->mHasColor ? "#define HAS_COLOR\n" : "");
        prefixFragment.append(parameters->mHasDiffuseMap ? "#define HAS_DIFFUSE_MAP\n" : "");
        prefixFragment.append(parameters->mHasEnvCubeMap ? "#define USE_ENVMAP\n" : "");
        prefixFragment.append(parameters->mHasSpecularMap ? "#define USE_SPECULARMAP\n" : "");

        prefixFragment.append(parameters->mShadowMapEnabled ? "#define USE_SHADOWMAP\n" : "");
        prefixFragment.append(parameters->mDepthPacking == DepthMaterial::RGBADepthPacking ? "#define DEPTH_PACKING_RGBA\n" : "");
        prefixFragment.append(parameters->mUseNormalMap ? "#define USE_NORMALMAP\n" : "");
        prefixFragment.append(parameters->mUseTangent ? "#define USE_TANGENT\n" : "");

        // 4 从parameters里面取出vs/fs基础功能代码
        auto vertexString = parameters->mVertex;
        auto fragmentString = parameters->mFragment;

        replaceAttributeLocation(versionString);
        replaceAttributeLocation(fragmentString);

        replaceLightNumbers(versionString, parameters);
        replaceLightNumbers(fragmentString, parameters);

        // 版本，扩展，前缀prefix(define各种功能的开启) + 本体shader
        vertexString = versionString + extensionString + prefixVertex + vertexString;
        fragmentString = versionString + extensionString + prefixFragment + fragmentString;

        auto vertex = versionString.c_str();
        auto fragment = fragmentString.c_str();

        std::cout << vertex << std::endl;
        std::cout << fragment << std::endl;
        std::cout << "---------------------------------------------" << std::endl;
        std::cout << std::endl;
        std::cout << std::endl;

        // shader的编译与链接
        uint32_t vertexID = 0, fragID = 0;
        char infoLog[512];
        int successFlag = 0;

        vertexID = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexID, 1, &vertex, nullptr);
        glCompileShader(vertexID);

        // 获取错误信息
        glGetShaderiv(vertexID, GL_COMPILE_STATUS, &successFlag);
        if (!successFlag)
        {
            glGetShaderInfoLog(vertexID, 512, nullptr, infoLog);
            std::cout << infoLog << std::endl;
        }

        fragID = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragID, 1, &fragment, nullptr);
        glCompileShader(fragID);

        glGetShaderiv(fragID, GL_COMPILE_STATUS, &successFlag);
        if (!successFlag)
        {
            glGetShaderInfoLog(fragID, 512, nullptr, infoLog);
            std::cout << infoLog << std::endl;
        }

        // 链接
        mProgram = glCreateProgram();
        glAttachShader(mProgram, vertexID);
        glAttachShader(mProgram, fragID);
        glLinkProgram(mProgram);

        glGetProgramiv(mProgram, GL_LINK_STATUS, &successFlag);
        if (!successFlag)
        {
            glGetProgramInfoLog(mProgram, 512, nullptr, infoLog);
            std::cout << infoLog << std::endl;
        }
        glDeleteShader(vertexID);
        glDeleteShader(fragID);

//        DebugLog::getInstance()->beginPrintUniformInfo(parameters->mShaderID);
        mUniforms = DriverUniforms::create(mProgram);
//        DebugLog::getInstance()->end();
    }

    DriverProgram::~DriverProgram() noexcept {
        glDeleteProgram(mProgram);
    }

    void DriverProgram::replaceAttributeLocation(std::string &shader) noexcept {
        // 1 通过正则表达式匹配相应的占位符
        // 2 匹配成功后，replace功能来将POSITION_LOCATION字符串替换为0

        // pattern-replace
        std::unordered_map<std::string, std::string> replaceMap = {
                {"POSITION_LOCATION", std::to_string(LOCATION_MAP.at("position"))},
                {"NORMAL_LOCATION", std::to_string(LOCATION_MAP.at("normal"))},
                {"UV_LOCATION", std::to_string(LOCATION_MAP.at("uv"))},
                {"COLOR_LOCATION", std::to_string(LOCATION_MAP.at("color"))},
                {"SKINNING_INDICES_LOCATION", std::to_string(LOCATION_MAP.at("skinIndex"))},
                {"SKINNING_WEIGHTS_LOCATION", std::to_string(LOCATION_MAP.at("skinWeight"))},
                {"TANGNENT_LOCATION", std::to_string(LOCATION_MAP.at("tangent"))},
                {"BITANGENT_B_LOCATION", std::to_string(LOCATION_MAP.at("bitangent"))},
        };

        for (const auto& iter : replaceMap) {
            // iter.first = 占位符字符串
            // iter.second = location

            // 使用C++的正则表达式进行替换，直接使用占位符的字符串初始化leregex
            std::regex pattern(iter.first);

            // 扫描整个shader字符串，只要发现符合pattern的字符串，就要替换为iter.second
            shader = std::regex_replace(shader, pattern, iter.second);
        }
    }

    void DriverProgram::replaceLightNumbers(std::string &shader, const Parameters::Ptr &parameters) noexcept {
        // pattern-replace
        std::unordered_map<std::string, std::string> replaceMap = {
                {"NUM_DIR_LIGHTS", std::to_string(parameters->mDirectionalLightCount)},
                {"NUM_DIR_LIGHT_SHADOWS", std::to_string(parameters->mNumDirectionalLightShadow)},
        };

        for (const auto& iter : replaceMap) {
            std::regex pattern(iter.first);
            shader = std::regex_replace(shader, pattern, iter.second);
        }
    }

    std::string DriverProgram::getExtensionString() noexcept {
        std::string extensionString = "";
        extensionString.append("#extension GL_ARB_separate_shader_objects : enable\n");

        return extensionString;
    }

    void DriverProgram::uploadUniforms(gx::UniformHandleMap &uniformHandleMap, const DriverTextures::Ptr &textures) {
        mUniforms->upload(uniformHandleMap, textures);
    }

    // ---------driverPrograms---------------------
    DriverPrograms::DriverPrograms() noexcept {}

    DriverPrograms::~DriverPrograms() noexcept {}

    DriverProgram::Ptr DriverPrograms::acquireProgram(const DriverProgram::Parameters::Ptr &parameters, HashType cacheKey) noexcept {
        // 虽然在当前Material对应的DriverMaterial当中，并没有找到曾经使用过的符合benParameters的DriverProgram
        // 但是从全局角度，其它Material可能曾经使用过符合本Parameters的DriverProgram
        auto iter = mPrograms.find(cacheKey);

        if (iter != mPrograms.end()) {
            return iter->second;
        }

        auto program = DriverProgram::create(parameters);
        program->mCacheKey = cacheKey;
        mPrograms.insert(std::make_pair(cacheKey, program));
        // 一旦调用本函数，则外部肯定有一个renderItem需要引用本program
        // 可能有多个material都曾经使用过当前这个DriverProgram，假设当前存活两个物体，两个不同的material，
        // 而且假设这两个material都使用了本DriverProgram，那么RefCount就是2
        program->mRefCount++;

        return program;
    }

    // 本函数被调用意味着外界某个renderItem释放了对本DriverProgram的使用
    void DriverPrograms::release(const DriverProgram::Ptr &program) noexcept {
        if (--program->mRefCount == 0) {
            mPrograms.erase(program->mCacheKey);
        }
    }

    DriverProgram::Parameters::Ptr DriverPrograms::getParameters(
            const Material::Ptr &material,
            const Object3D::Ptr &object,
            const DriverLights::Ptr &lights,
            const DriverShadowMap::Ptr &shadowMap
    ) noexcept {
        auto renderObject = std::static_pointer_cast<RenderableObject>(object);
        auto geometry = renderObject->getGeometry();

        // 新建一个parameters
        auto parameters = DriverProgram::Parameters::create();

        auto shaderID = material->getType();
        auto shaderIter = ShaderLib.find(shaderID);

        if (shaderIter == ShaderLib.end()) {
            return nullptr;
        }

        parameters->mShaderID = material->getType();
        // shaderIter->second 即 shader struct object
        parameters->mVertex = shaderIter->second.mVertex;
        parameters->mFragment = shaderIter->second.mFragment;

//        parameters->mShadowMapEnabled = shadowMap---------------------------------------------------------------------------
        parameters->mDirectionalLightCount = lights->mState.mDirectionalCount;
        parameters->mNumDirectionalLightShadow = lights->mState.mNumDirectionalShadows;

        if (geometry->hasAttribute("normal")) {
            parameters->mHasNormal = true;
        }

        if (geometry->hasAttribute("uv")) {
            parameters->mHasUV = true;
        }

        if (geometry->hasAttribute("color")) {
            parameters->mHasColor = true;
        }

        if (material->mDiffuseMap != nullptr) {
            parameters->mHasDiffuseMap = true;
        }

        if (material->mEnvMap != nullptr) {
            parameters->mHasEnvCubeMap = true;
        }

        if (material->mSpecularMap != nullptr) {
            parameters->mHasSpecularMap = true;
        }

        if (material->mNormalMap != nullptr) {
            parameters->mHasNormal = true;
        }

        if (object->mIsRenderableObject) {
            auto renderableObject = std::static_pointer_cast<RenderableObject>(object);
            auto geometry = renderableObject->getGeometry();
            parameters->mUseTangent = geometry->hasAttribute("tangent");
        }

        if (material->mIsDepthMaterial) {
            auto depthMaterial = std::static_pointer_cast<DepthMaterial>(material);
            parameters->mDepthPacking = depthMaterial->mPacking;
        }

        if (object->mIsSkinnedMesh) {
        }

    }

    UniformHandleMap DriverPrograms::getUniforms(const Material::Ptr &material) noexcept {
        UniformHandleMap uniforms{};

        auto shaderID = material->getType();
        auto shaderIter = ShaderLib.find(shaderID);

        if (shaderIter != ShaderLib.end()) {
            uniforms = shaderIter->second.mUniformMap;
        }

        return uniforms;
    }

    // 将parameters做成字符串，然后进行哈希运算，得到最终的哈希结果
    HashType DriverPrograms::getProgramCacheKey(const DriverProgram::Parameters::Ptr &parameters) noexcept {
        std::hash<std::string> hasher;

        std::string keyString;

        keyString.append(parameters->mShaderID);
        keyString.append(parameters->mVertex);
        keyString.append(parameters->mFragment);
        keyString.append(std::to_string(parameters->mHasNormal));
        keyString.append(std::to_string(parameters->mHasUV));
        keyString.append(std::to_string(parameters->mHasColor));
        keyString.append(std::to_string(parameters->mHasDiffuseMap));
        keyString.append(std::to_string(parameters->mHasEnvCubeMap));
        keyString.append(std::to_string(parameters->mHasSpecularMap));
        keyString.append(std::to_string(parameters->mDirectionalLightCount));
        keyString.append(std::to_string(parameters->mNumDirectionalLightShadow));
        keyString.append(std::to_string(parameters->mSkinning));
        keyString.append(std::to_string(parameters->mMaxBones));
        keyString.append(std::to_string(parameters->mUseNormalMap));
        keyString.append(std::to_string(parameters->mUseTangent));
        keyString.append(std::to_string(parameters->mDepthPacking));

        return hasher(keyString);
    }
}