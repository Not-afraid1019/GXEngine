#include "driverUniforms.h"
#include "../../warpper/glWrapper.hpp"


namespace gx {

    UniformBase::UniformBase() noexcept {}

    UniformBase::~UniformBase() noexcept {}

    SingleUniform::SingleUniform(const std::string &id, const GLuint &location, const GLenum &type) noexcept {
        mLocation = location;
        mID = id;
        mType = type;
    }

    SingleUniform::~SingleUniform() noexcept {}

#define UPLOAD(TYPE, VALUE) \
    {\
        TYPE v = std::any_cast<TYPE>(VALUE);\
        upload(v);\
    }

#define UPLOAD_ARRAY(TYPE, VALUE) \
    {\
        auto v = std::any_cast<std::vector<TYPE>>(VALUE);\
        upload(static_cast<TYPE*>(v.data()));\
    }

    void SingleUniform::setValue(
            const std::any &value,
            const DriverTextures::Ptr &textures,
            const std::shared_ptr<DriverUniforms> &driverUniforms)
        {
            // 根据不同的uniform变量类型，做不同的处理
            switch (mType) {
                case GL_FLOAT:
                    UPLOAD(float, value)
                    break;
                case GL_FLOAT_VEC2:
                    UPLOAD(glm::vec2, value)
                    break;
                case GL_FLOAT_VEC3:
                    UPLOAD(glm::vec3, value)
                    break;
                case GL_FLOAT_VEC4:
                    UPLOAD(glm::vec4, value)
                    break;
                case GL_INT:
                    UPLOAD(int, value)
                    break;
                case GL_INT_VEC2:
                    UPLOAD(glm::ivec2, value)
                    break;
                case GL_INT_VEC3:
                    UPLOAD(glm::ivec3, value)
                    break;
                case GL_INT_VEC4:
                    UPLOAD(glm::ivec4, value)
                    break;
                case GL_BOOL:
                    UPLOAD(bool, value)
                    break;
                case GL_BOOL_VEC2:
                    UPLOAD(glm::bvec2, value)
                    break;
                case GL_BOOL_VEC3:
                    UPLOAD(glm::bvec3, value)
                    break;
                case GL_BOOL_VEC4:
                    UPLOAD(glm::bvec4, value)
                    break;
                case GL_FLOAT_MAT2:
                    UPLOAD(glm::mat2, value)
                    break;
                case GL_FLOAT_MAT3:
                    UPLOAD(glm::mat3, value)
                    break;
                case GL_FLOAT_MAT4:
                    UPLOAD(glm::mat4, value)
                    break;
                case GL_SAMPLER_2D:
                    uploadTexture(driverUniforms, textures, value);
                    break;
                case GL_SAMPLER_CUBE:
                    uploadTexture(driverUniforms, textures, value);
                    break;
                default:
                    break;
            }
        }

    void SingleUniform::uploadTexture(
            const std::shared_ptr<DriverUniforms> &driverUniforms,
            const DriverTextures::Ptr &textures,
            const std::any &value)
    {

        Texture::Ptr texture = nullptr;
        if (value.type() == typeid(Texture::Ptr)) {
            texture = std::any_cast<Texture::Ptr>(value);
        }
        else if (value.type() == typeid(CubeTexture::Ptr)) {
            texture = std::any_cast<CubeTexture::Ptr>(value);
        }

        // 当前的SingleUniform一定是一个sampler2D的uniform
        // 在singleUniform初始化创建的时候，就已经拿到了自己的location
        // 从DriverUniforms里面，试图获取本uniform所对应的textureUnit
        auto textureSlot = driverUniforms->getTextureSlot(mLocation);

        // 如果还没有给当前的texture uniform分配textureUnit，那么就得重新分配一个
        if (textureSlot < 0) {
            textureSlot = driverUniforms->allocateTextureUnits(1)[0];

            // 分配之后，需要在driverUniforms里面记录下来
            driverUniforms->setTextureSlot(mLocation, textureSlot);
        }

        // 将新获得或者已经分配完毕的textureUnit与当前texture对应的DriverTexture中的mHandle进行绑定
        textures->bindTexture(texture, textureSlot);

        // 将shader中的sampler与textureUnit进行绑定
        // 传输给sampler的是一个整数
        GLint textureIndex = textureSlot - GL_TEXTURE0;
        gl::uniform1i(mLocation, textureIndex);
    }

    PureArrayUniform::PureArrayUniform(const std::string &id, const GLint &location, const GLenum &type, GLint size) noexcept {
        mLocation = location;
        mID = id;
        mType = type;
        mSize = size;
    }

    PureArrayUniform::~PureArrayUniform() noexcept {}

    void PureArrayUniform::setValue(
            const std::any &value,
            const DriverTextures::Ptr &textures,
            const std::shared_ptr<DriverUniforms> &driverUniforms)
    {
        switch (mType) {
            case GL_FLOAT:
                UPLOAD_ARRAY(float, value)
                break;
            case GL_FLOAT_VEC2:
                UPLOAD_ARRAY(glm::vec2, value)
                break;
            case GL_FLOAT_VEC3:
                UPLOAD_ARRAY(glm::vec3, value)
                break;
            case GL_FLOAT_VEC4:
                UPLOAD_ARRAY(glm::vec4, value)
                break;
            case GL_INT:
                UPLOAD_ARRAY(int, value)
                break;
            case GL_INT_VEC2:
                UPLOAD_ARRAY(glm::ivec2, value)
                break;
            case GL_INT_VEC3:
                UPLOAD_ARRAY(glm::ivec3, value)
                break;
            case GL_INT_VEC4:
                UPLOAD_ARRAY(glm::ivec4, value)
                break;
            case GL_BOOL:
                UPLOAD_ARRAY(int, value)
                break;
            case GL_BOOL_VEC2:
                UPLOAD_ARRAY(glm::ivec2, value)
                break;
            case GL_BOOL_VEC3:
                UPLOAD_ARRAY(glm::ivec3, value)
                break;
            case GL_BOOL_VEC4:
                UPLOAD_ARRAY(glm::ivec4, value)
                break;
            case GL_FLOAT_MAT2:
                UPLOAD_ARRAY(glm::mat2, value)
                break;
            case GL_FLOAT_MAT3:
                UPLOAD_ARRAY(glm::mat3, value)
                break;
            case GL_FLOAT_MAT4:
                UPLOAD_ARRAY(glm::mat4, value)
                break;
            case GL_SAMPLER_2D:
                uploadTexture2DArray(driverUniforms, textures, value);
                break;
            default:
                break;
        }
    }

    void PureArrayUniform::uploadTexture2DArray(
            const std::shared_ptr<DriverUniforms> &driverUniforms,
            const DriverTextures::Ptr &textures,
            const std::any &value)
    {
        auto textureArray = std::any_cast<std::vector<Texture::Ptr>>(value);

        std::vector<GLint> textureSlots = driverUniforms->getTextureArraySlot(mLocation);
        if (textureSlots.size() == 0) {
            textureSlots = driverUniforms->allocateTextureUnits(mSize);
            driverUniforms->setTextureArraySlot(mLocation, textureSlots);
        }

        std::vector<GLint> textureIndices;
        for (int i = 0; i < textureSlots.size(); ++i) {
            textureIndices.push_back(textureSlots[i] - GL_TEXTURE0);
        }

        for (uint32_t i = 0; i < textureArray.size(); ++i) {
            textures->bindTexture(textureArray[i], textureSlots[i]);
        }

        gl::uniform1iv(mLocation, textureArray.size(), textureIndices.data());
    }


    StructuredUniform::StructuredUniform(const std::string &id) noexcept {
        mID = id;
    }

    StructuredUniform::~StructuredUniform() noexcept {}

    void StructuredUniform::setValue(
            const std::any &value,
            const DriverTextures::Ptr &textures,
            const std::shared_ptr<DriverUniforms> &driverUniforms)
    {
        auto v = std::any_cast<UniformUnitMap>(value);

        // 遍历的是已经解析出来的uniformMap
        for (const auto& iter : mUniformMap) {
            auto name = iter.first;
            auto uniform = iter.second;

            uniform->setValue(v[name], textures, driverUniforms);
        }
    }

    // 收到了外界给予的program，解析其中的activeUniforms，拿出相关信息
    // 每拿出来一个信息，都要进行正则表达式匹配，从而对应生成三种uniforms当中的一种
    // 在这个过程中，形成了uniforms的层级架构
    DriverUniforms::DriverUniforms(const GLuint &program) noexcept : UniformContainer() {
        GLint count = 0;
        glGetProgramiv(program, GL_ACTIVE_UNIFORMS, &count);

        // can not use sharedPtr
        UniformContainer* container = this;

        GLint location = 0;
        GLsizei bufferSize = 256;
        GLsizei length;
        GLint size;
        GLenum type;
        GLchar name[256];

        for (uint32_t i = 0; i < count; ++i) {
            glGetActiveUniform(program, i, bufferSize, &length, &size, &type, name);
            location = glGetUniformLocation(program, name);

            std::string pattern = "(\\w+)(\\])?)(\\[|\\.)?";
            std::regex reg(pattern);

            std::smatch result;

            std::string text = name;
            std::string id;
            std::string subscript;
            bool idIsIndex{false};
            size_t matchEnd{0};

            // don't forget
            container = this;

            // if it is an pureArray, then OpenGL gets xxx[0], so matchEnd is xxx[
            while (true) {
                if (std::regex_search(text, result, reg)) {
                    id = result[1].str();
                    idIsIndex = (result[2].str() == "]");
                    subscript = result[3].str();
                    matchEnd = result.position(0) + result.length(0);

                    if (subscript.empty() || (subscript == "[" && matchEnd + 2 == text.length())) {
                        UniformBase::Ptr uniformObject = nullptr;

                        if (subscript.empty()) {
                            uniformObject = SingleUniform::create(id, location, type);
                        }
                        else {
                            uniformObject = PureArrayUniform::create(id, location, type, size);
                        }

                        addUniform(container, uniformObject);
                        break;
                    }
                    else {
                        StructuredUniform::Ptr next = nullptr;

                        auto uniformMap = container->mUniformMap;
                        auto iter = uniformMap.find(id);

                        if (iter == uniformMap.end()) {
                            next = StructuredUniform::create(id);
                            addUniform(container, next);
                        }
                        else {
                            next = std::dynamic_pointer_cast<StructuredUniform>(iter->second);
                        }

                        container = next.get();
                    }

                    text = result.suffix().str();
                }
                else {
                    break;
                }
            }

//            DebugLog::getInstance()->printUniformInfo(i, length, size, type, name);
        }
    }

    DriverUniforms::~DriverUniforms() noexcept {}

    void DriverUniforms::upload(gx::UniformHandleMap &uniformHandleMap, const DriverTextures::Ptr &textures) {
        // 遍历的标准是DriverUniforms内部已经解析好的Uniforms架构
        for (auto& iter : mUniformMap) {
            auto name = iter.first;
            auto uniform = iter.second; // UniformBase,可能是三种类型之一

            // 取出来的UniformHandle
            auto& uniformHandle = uniformHandleMap[name];

            if (uniformHandle.mNeedsUpdate) {
                uniformHandle.mNeedsUpdate = false;

                // single pureArray structured 三种uniform都各自实现了各自的setValue
                // 如果是single，mValue可能是个matrix也可能是个float等等
                // 如果是pureArray，mValue必须是一个vector<T>
                // 如果是structured，mValue必须是一个UniformUnitMap
                uniform->setValue(uniformHandle.mValue, textures, shared_from_this());
            }
        }
    }

    void DriverUniforms::addUniform(gx::UniformContainer *container, const UniformBase::Ptr &uniformObject) {
        container->mUniformMap.insert(std::make_pair(uniformObject->mID, uniformObject));
    }

    void DriverUniforms::setTextureSlot(const GLint &location, GLuint slot) noexcept {
        mTextureSlots.insert(std::make_pair(location, slot));
    }

    GLint DriverUniforms::getTextureSlot(const GLint &location) noexcept {
        auto iter = mTextureSlots.find(location);
        if (iter != mTextureSlots.end()) {
            return iter->second;
        }

        return -1;
    }

    std::vector<GLint> DriverUniforms::getTextureArraySlot(const GLint &location) noexcept {
        std::vector<GLint> slots;
        auto iter = mTextureArraySlots.find(location);
        if (iter != mTextureArraySlots.end()) {
            slots = iter->second;
        }

        return slots;
    }

    std::vector<GLint> DriverUniforms::allocateTextureUnits(const int &n) {
        std::vector<GLint> units;
        for (int i = 0; i < n; ++i) {
            GLenum slot = GL_TEXTURE0 + mCurrentTextureSlots;
            if (slot >= MAX_TEXTURE) {
                throw std::runtime_error("DriverTextures->allocateTextureUnit: too much textures");
            }

            units.push_back(slot);
            mCurrentTextureSlots++;
        }

        return units;
    }
}