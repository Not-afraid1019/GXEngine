#include "gx/core/attribute.h"
#include "gx/core/geometry.h"
#include "gx/objects/mesh.h"
#include "gx/scene/scene.h"
#include "gx/camera/perspectiveCamera.h"
#include "gx/cameraControl/gameCameraControl.h"
#include "gx/render/renderer.h"
#include "gx/material/meshBasicMaterial.h"
#include "gx/log/debugLog.h"
#include "gx/global/constant.h"


// 窗体的大小
uint32_t WIDTH = 800;
uint32_t HEIGHT = 600;

// 响应鼠标移动，收到的参数为鼠标当前位置
static void onMouseMove(double xpos, double ypos) {}


// 响应鼠标事件，比如点击或者抬起
static void onMouseAction(gx::MouseAction action) {
    if (action == gx::MouseAction::LeftDown) {
        std::cout << "LeftDown" << std::endl;
    }
}

// 响应键盘事件，并且回传键盘状态
static void onKeyboardAction(KeyBoardState action) {}


// 响应窗口大小变化，并且回传变化后的窗体宽高
static void onResize(int width, int height) {}


int main() {

    std::vector<float> positions = {
            -0.5f, 0.0f, 0.0f,
            0.0f, 0.5f, 0.0f,
            0.5f, 0.0f, 0.0f
    };

    std::vector<float> colors = {
            1.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 1.0f
    };

    std::vector<uint32_t> indices = {
            0, 1, 2
    };

    // 创造几何数据，并且为Mesh的顶点们，设置属性数据Attribute
    // 其中itemSize是指每个顶点相关属性有多少个数字
    auto geometry = gx::Geometry::create();
    geometry->setAttribute("position", gx::Attributef::create(positions, 3));
    geometry->setAttribute("color", gx::Attributef::create(colors, 3));
    geometry->setIndex(gx::Attributei::create(indices, 1));

    // 生成基础材质
    auto material = gx::MeshBasicMaterial::create();

    // 一个Mesh，包括它的几何信息，以及材质信息
    auto triangle = gx::Mesh::create(geometry, material);

    //创建场景，并且将三角形Mesh加入其中
    auto scene = gx::Scene::create();
    scene->addChild(triangle);

    // 创建透视投影摄像机near far 宽高比 视角 位置
    auto camera = gx::PerspectiveCamera::create(0.1f, 100.0f, (float)WIDTH / (float)HEIGHT, 60.0f);
    camera->setPosition(0.0f, 0.0f, 1.0f);

    // 创建渲染器
    gx::Renderer::Descriptor rDc;
    rDc.mWidth = WIDTH;
    rDc.mHeight = HEIGHT;
    gx::Renderer::Ptr renderer = gx::Renderer::create(rDc);

    // 设置背景清除色，类似于glClearColor
    renderer->setClearColor(0.94, 1.0, 0.94, 1.0);

    renderer->setMouseActionCallback(onMouseAction);
    renderer->setKeyboardActionCallback(onKeyboardAction);
    renderer->setFrameSizeCallback(onResize);
    renderer->setMouseMoveCallback(onMouseMove);

    while (true) {
        if (!renderer->render(scene, camera)) {
            break;
        }

        renderer->swap();
    }
}