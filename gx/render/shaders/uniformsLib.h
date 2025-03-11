#pragma once
#include "../../global/base.h"
#include "../../global/constant.h"
#include "../../textures/texture.h"
#include "../../textures/cubeTexture.h"

namespace gx {

    // 在绘制一个物体的时候，即做一次DrawCall的时候，就必须更新使用的Shader所必要的Uniforms
    // 将所需要更新的所有的Uniforms的数据放在了统一的Map当中
    // 本Map的key就是需要更新的Uniform的名字 (跟Shader里面的那个Uniform的名字一一对应)，对于PureArray与Single
    // 对于StructuredUniforms，是按照层级的架构来选择名称
    // 例如
    // 比如除了基础矩阵之外，还存在如下结构体
    //struct Test {
    //    float a;
    //    float b;
    //};
    //
    //uniform Test t;
    //
    // std::unordered_map<std::string, UniformHandle> outMap
    // outMap["modelViewMatrix"] = UniformHandle(mValue = object->getModelViewMatrix())
    // outMap["normalMatrix"] = UniformHandle(mValue = object->getNormalMatrix())
    //
    // std::unordered_map<std::string, std::any> contents;
    // contents["a"] = 0.0;
    // contents["b"] = 0.5;
    //
    // UniformHandle* tHandle = UniformHandle();
    // tHandle.mValue = contents;
    //
    // outMap["t"] = tHandle;
    //
    // 最终这个outMap就可以传入DriverUniforms的upload里面，进行更新
    //
    class UniformHandle {
    public:
        UniformHandle() noexcept {}

        ~UniformHandle() noexcept {}

        std::any    mValue; // 可以是任何类型的值，比如float matrix map
        bool mNeedsUpdate{false};
    };

    // for non-first-level uniform
    using UniformUnitMap = std::unordered_map<std::string, std::any>;

    // for first-level uniform
    using UniformHandleMap = std::unordered_map<std::string, UniformHandle>;

    static const std::unordered_map<std::string, UniformHandleMap> UniformsLib =
            {
                    {
                        "common", {
                            {"diffuseMap", UniformHandle()},
                            {"opacity", UniformHandle()}
                        }
                    },
                    {
                        "envMap", {
                              {"envMap", UniformHandle()}
                        }
                    },
                    {
                        "specularMap", {
                               {"specularMap", UniformHandle()}
                        }
                    },
                    {
                        "normalMap", {
                                {"normalMap", UniformHandle()}
                        }
                    }

            };

    // --------------tools------------------
    static UniformHandleMap merge(const std::vector<UniformHandleMap>& uniformGroups) {
        UniformHandleMap mergedGroup;

        for (const auto& group : uniformGroups) {
            for (const auto& uniformIter : group) {
                auto name = uniformIter.first;
                auto handle = uniformIter.second;

                mergedGroup.insert(std::make_pair(name, handle));
            }
        }

        return std::move(mergedGroup);
    }

    static UniformUnitMap* setArrayStructuredUniform(const uint32_t& index, UniformUnitMap* array) noexcept {
        auto indexString = std::to_string(index);
        (*array)[indexString] = UniformUnitMap();

        return std::any_cast<UniformUnitMap>(&(*array)[indexString]);
    }

    static UniformUnitMap* getArrayStructuredUniform(const uint32_t& index, UniformUnitMap* array) noexcept {
        auto indexString = std::to_string(index);
        return std::any_cast<UniformUnitMap>(&(*array)[indexString]);
    }

    template<typename T>
    static void clearPureArrayUniform(std::vector<T>* array) {
        array->clear();
    }

    template<typename T>
    static void pushPureArrayUniform(T value, std::vector<T>* array) {
        array->push_back(value);
    }

    template<typename T>
    static void setPureArrayUniform(const uint32_t& index, T value, std::vector<T>* array) {
        if (index >= array->size()) {
            throw std::runtime_error("Error: index is bigger than pure array uniform's bound");
        }
        (*array)[index] = value;
    }

    template<typename T>
    static T getPureArrayUniform(const uint32_t& index, T value, std::vector<T>* array) {
        if (index >= array->size()) {
            throw std::runtime_error("Error: index is bigger than pure array uniform's bound");
        }
        return (*array)[index] = value;
    }
}