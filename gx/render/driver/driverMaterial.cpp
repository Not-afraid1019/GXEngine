#include "driverMaterials.h"

namespace gx {


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