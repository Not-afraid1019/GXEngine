#include "renderer.h"
#include "../tools/timer.h"

namespace gx {

    Timer timer;

    static void onFrameSizeCallback(DriverWindow* dwindow, int width, int height) {
        if (dwindow->mRenderer != nullptr) {
            dwindow->mRenderer->setSize(width, height);
        }
    }

    Renderer::Renderer(const gx::Renderer::Descriptor &descriptor) noexcept {

    }

    Renderer::~Renderer() noexcept {}

























    bool Renderer::render(Scene::Ptr scene, Camera::Ptr camera) {
        // 处理按键消息，并且判断是否继续render
        if (mWindow->processEvent()) {
            return false;
        }

    }
}