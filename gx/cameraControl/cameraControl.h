#pragma once
#include "../global/base.h"
#include "../global/constant.h"
#include "../camera/camera.h"

namespace gx {

    // 任务:根据键盘鼠标,更改camera的矩阵
    // 种类:gameCameraControl wasd 鼠标右键 trackBallControl cad/cae软件
    class CameraControl {
    public:
        using Ptr = std::shared_ptr<CameraControl>;

        CameraControl(const Camera::Ptr& camera) noexcept;

        ~CameraControl() noexcept;

        // 本质上来讲就是bit位集合，一共108键盘
        virtual void onKeyboard(const KeyBoardState& action) noexcept;

        virtual void onMouseAction(const MouseAction& action) noexcept;

        virtual void onMouseMove(double xpos, double ypos) noexcept;

    protected:
        Camera::Ptr mCamera{nullptr};
    };
}
