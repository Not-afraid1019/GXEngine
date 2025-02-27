#include "cameraControl.h"

namespace gx {

    CameraControl::CameraControl(const Camera::Ptr &camera) noexcept {
        mCamera = camera;
    }

    CameraControl::~CameraControl() noexcept = default;

    void CameraControl::onKeyboard(const KeyBoardState &action) noexcept {

    }

    void CameraControl::onMouseAction(const gx::MouseAction &action) noexcept {

    }

    void CameraControl::onMouseMove(double xpos, double ypos) noexcept {

    }
}