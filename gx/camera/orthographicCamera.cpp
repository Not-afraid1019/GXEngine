#include "orthographicCamera.h"

namespace gx {

    OrthographicCamara::OrthographicCamara(float left, float right, float bottom, float top, float near, float far) noexcept {
        mIsOrthographicCamera = true;
        mLeft = left;
        mRight = right;
        mBottom = bottom;
        mTop = top;
        mNear = near;
        mFar = far;

        updateProjectionMatrix();
    }

    OrthographicCamara::~OrthographicCamara() noexcept {}

    glm::mat4 OrthographicCamara::updateProjectionMatrix() noexcept {
        mProjectionMatrix = glm::ortho(mLeft, mRight, mBottom, mTop, mNear, mFar);
        return mProjectionMatrix;
    }
}