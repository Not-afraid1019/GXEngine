#include "driverMaterials.h"
#include "../../textures/texture.h"

namespace gx {

    DriverMaterial::DriverMaterial() noexcept {}

    DriverMaterial::~DriverMaterial() noexcept {}

    DriverMaterials::DriverMaterials(const DriverPrograms::Ptr &programs) noexcept {
        mPrograms = programs;
        EventDispatcher::getInstance()->addEventListener("materialDispose", this, &DriverMaterials::onMaterialDispose);
    }

    DriverMaterials::~DriverMaterials() noexcept {
        EventDispatcher::getInstance()->removeEventListener("materialDispose", this, &DriverMaterials::onMaterialDispose);
    }

    DriverMaterial::Ptr DriverMaterials::get(const Material::Ptr material) noexcept {
        auto iter = mMaterials.find(material->getID());

        if (iter == mMaterials.end()) {
            iter = mMaterials.insert(std::make_pair(material->getID(), DriverMaterial::create())).first;
        }

        return iter->second;
    }

    void DriverMaterials::onMaterialDispose(const EventBase::Ptr &event) {
        auto material = (Material*)event->mTarget;

        // 比如生成了一个material但是并没有使用，然后就析构了
        auto iter = mMaterials.find(material->getID());
        if (iter == mMaterials.end()) {
            return;
        }

        auto dMaterial = iter->second;

        // 拿到当前material曾经使用过的所有DriverPrograms
        auto programs = dMaterial->mPrograms;
        for (const auto& pIter : programs) {
            auto program = pIter.second;

            // program是DriverProgram
            // mPrograms是DriverPrograms
            mPrograms->release(program);
        }

        mMaterials.erase(iter);
    }

    void DriverMaterials::refreshMaterialUniforms(gx::UniformHandleMap &uniformHandleMap, const Material::Ptr &material) {
        uniformHandleMap["opacity"].mValue = material->mOpacity;
        uniformHandleMap["opacity"].mNeedsUpdate = true;


        if (material->mIsMeshBasicMaterial) {
            auto basicMaterial = std::static_pointer_cast<MeshBasicMaterial>(material);
            refreshMaterialBasic(uniformHandleMap, basicMaterial);
        }

        if (material->mIsMeshPhongMaterial) {
            auto phongMaterial = std::static_pointer_cast<MeshPhongMaterial>(material);
            refreshMaterialPhong(uniformHandleMap, phongMaterial);
        }

        if (material->mIsCubeMaterial) {
            auto cubeMaterial = std::static_pointer_cast<CubeMaterial>(material);
            refreshMaterialCube(uniformHandleMap, cubeMaterial);
        }
    }

    void DriverMaterials::refreshMaterialPhong(gx::UniformHandleMap &uniformHandleMap, const MeshPhongMaterial::Ptr &material) {
        uniformHandleMap["shininess"].mValue = material->mShininess;
        uniformHandleMap["shininess"].mNeedsUpdate = true;

        if ((material->mDiffuseMap && material->mDiffuseMap->mNeedsUpdate) || material->mNeedsUpdate) {
            uniformHandleMap["diffuseMap"].mValue = material->mDiffuseMap;
            uniformHandleMap["diffuseMap"].mNeedsUpdate = true;
        }

        if ((material->mNormalMap && material->mNormalMap->mNeedsUpdate) || material->mNeedsUpdate) {
            uniformHandleMap["normalMap"].mValue = material->mNormalMap;
            uniformHandleMap["normalMap"].mNeedsUpdate = true;
        }

        if ((material->mSpecularMap && material->mSpecularMap->mNeedsUpdate) || material->mNeedsUpdate) {
            uniformHandleMap["specularMap"].mValue = material->mSpecularMap;
            uniformHandleMap["specularMap"].mNeedsUpdate = true;
        }
    }

    void DriverMaterials::refreshMaterialBasic(gx::UniformHandleMap &uniformHandleMap, const MeshBasicMaterial::Ptr &material) {
        if ((material->mDiffuseMap && material->mDiffuseMap->mNeedsUpdate) || material->mNeedsUpdate) {
            uniformHandleMap["diffuseMap"].mValue = material->mDiffuseMap;
            uniformHandleMap["diffuseMap"].mNeedsUpdate = true;
        }
    }

    void DriverMaterials::refreshMaterialCube(gx::UniformHandleMap &uniformHandleMap, const CubeMaterial::Ptr &material) {
        if ((material->mEnvMap && material->mEnvMap->mNeedsUpdate) || material->mNeedsUpdate) {
            uniformHandleMap["envMap"].mValue = material->mEnvMap;
            uniformHandleMap["envMap"].mNeedsUpdate = true;
        }
    }
}