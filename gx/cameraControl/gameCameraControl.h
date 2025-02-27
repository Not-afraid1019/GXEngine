#pragma once
#include "cameraControl.h"
#include "../global/constant.h"

namespace gx {

    class GameCameraControl : public CameraControl {
    public:
        static constexpr int MoveFront = 0;
        static constexpr int MoveBack = 1;
        static constexpr int MoveLeft = 2;
        static constexpr int MoveRight = 3;

        using Ptr = std::shared_ptr<GameCameraControl>;
        static Ptr create(const Camera::Ptr& camera) {
            return std::make_shared<GameCameraControl>(camera);
        }

        GameCameraControl(const Camera::Ptr& camera) noexcept;

        ~GameCameraControl() noexcept;

        void setSpeed(float speed) noexcept;

        void setSensitivity(float s) noexcept;

        void onKeyboard(const KeyBoardState& action) noexcept override;

        void onMouseAction(const gx::MouseAction &action) noexcept override;

        void onMouseMove(double xpos, double ypos) noexcept override;

        void update() noexcept;

    private:
        float mSpeed{0.1f}; // 移动速度
        float mSensitivity{0.1f}; // 鼠标灵敏度

        glm::vec3 mFront = glm::vec3(0.0f); // 当前相机的朝向
        float mPitchAngle{0.0f};    // 俯仰角
        float mYawAngle{-90.0f};    // 左右摇头的角度

        // control parameters
        glm::vec2 mCurrentMousePosition = glm::vec2(0.0f);
        // 用来记录当前正在移动的方向,可以同时多个
        glm::bvec4 mMoveState = glm::bvec4(false);

        // 是否正在旋转,鼠标右键点住之后,可以拖动上下左右旋转视角,抬起之后就不能了
        bool mRotationState = false;
        bool mMouseStateReset = true;
    };
}
