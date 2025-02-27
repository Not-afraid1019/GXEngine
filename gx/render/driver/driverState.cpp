#include "driverState.h"
#include "../../warpper/glWrapper.hpp"

namespace gx {

    DriverState::DriverState() noexcept {}

    DriverState::~DriverState() noexcept {}

    void DriverState::viewport(const glm::vec4 &viewport) noexcept {
        mCurrentViewport = viewport;
    }













    bool DriverState::useProgram(GLuint program) noexcept {
        // 检查上一个物体渲染使用的Program是否跟当前的一样
        // 只有不一样的时候才会重新绑定
        if (mCurrentProgram != program) {
            glUseProgram(program);
            mCurrentProgram = program;

            return true;
        }

        return false;
    }

    void DriverState::setMaterial(const Material::Ptr &material) noexcept {
        // 对于双面渲染，有两种方案
        // 1 在绘制背面的时候，进行法线反转
        // 2 在绘制背面的时候，不进行法线反转(采用方案)
        if (mCurrentRaster.mSide != material->mSide) {
            if (material->mSide == Side::DoubleSide) {
                gl::disable(GL_CULL_FACE);
            }
            else {
                gl::enable(GL_CULL_FACE);
                glCullFace(toGL(material->mSide));
            }
        }

        if (mCurrentRaster.mFrontFace != material->mFrontFace) {
            mCurrentRaster.mFrontFace = material->mFrontFace;
            glFrontFace(toGL(material->mFrontFace));
        }

        setBlending(
                material->mBlendingType,
                material->mTransparent,
                material->mBlendSrc,
                material->mBlendDst,
                material->mBlendingEquation,
                material->mBlendSrcAlpha,
                material->mBlendDstAlpha,
                material->mBlendingEquationAlpha
                );

        setDepth(material->mDepthTest, material->mDepthWrite, material->mDepthFunction, material->mDepthClearColor);

    }

    void DriverState::setBlending(
            BlendingType blendingType,
            bool transparent,
            BlendingFactor blendSrc,
            BlendingFactor blendDst,
            BlendingEquation blendEquation,
            BlendingFactor blendSrcAlpha,
            BlendingFactor blendDstAlpha,
            BlendingEquation blendEquationAlpha
    ) noexcept {
        // 当前状态与material要求状态不一致，且不是用户自定义的blendingType
        if (mCurrentBlending.mBlendingType != blendingType && mCurrentBlending.mBlendingType != BlendingType::CustomBlending) {
            mCurrentBlending.mBlendingType = blendingType;

            if (blendingType == BlendingType::NoBlending) {
                glDisable(GL_BLEND);
                return;
            }

            if (mCurrentBlending.mBlendingType == BlendingType::DefaultBlending) {
                // 根据传入的参数，设置blending的所有状态
                setBlendingInternal(
                        true,
                        BlendingFactor::SrcAlpha,
                        BlendingFactor::OneMinusSrcAlpha,
                        BlendingEquation::AddEquation,
                        BlendingFactor::SrcAlpha,
                        BlendingFactor::OneMinusSrcAlpha,
                        BlendingEquation::AddEquation
                        );
                return;
            }

        }
        else if (mCurrentBlending.mBlendingType == BlendingType::CustomBlending) {
            setBlendingInternal(
                    transparent,
                    blendSrc,
                    blendDst,
                    blendEquation,
                    blendSrcAlpha,
                    blendDstAlpha,
                    blendEquationAlpha
                    );
        }
    }

    void DriverState::setBlendingInternal(
            bool transparent,
            BlendingFactor blendSrc,
            BlendingFactor blendDst,
            BlendingEquation blendEquation,
            BlendingFactor blendSrcAlpha,
            BlendingFactor blendDstAlpha,
            BlendingEquation blendEquationAlpha
    ) noexcept {
        if (!mCurrentBlending.mTransparent && !transparent) {
            return;
        }

        if (mCurrentBlending.mTransparent != transparent) {
            mCurrentBlending.mTransparent = transparent;
            if (transparent) {
                gl::enable(GL_BLEND);
            }
        }

        if (!transparent) {
            gl::disable(GL_BLEND);
            return;
        }

        // 但凡发现有不相等的变量，就重新记录，并调用API进行状态更改
        if (
            mCurrentBlending.mBlendSrc != blendSrc ||
            mCurrentBlending.mBlendDst != blendDst ||
            mCurrentBlending.mBlendSrcAlpha != blendSrcAlpha ||
            mCurrentBlending.mBlendDstAlpha != blendDstAlpha
            ) {

            mCurrentBlending.mBlendSrc = blendSrc;
            mCurrentBlending.mBlendDst = blendDst;
            mCurrentBlending.mBlendSrcAlpha = blendSrcAlpha;
            mCurrentBlending.mBlendDstAlpha = blendDstAlpha;

            gl::blendFuncSeparate(toGL(blendSrc), toGL(blendDst), toGL(blendSrcAlpha), toGL(blendDstAlpha));
        }

        if (
            mCurrentBlending.mBlendEquation != blendEquation ||
            mCurrentBlending.mBlendEquationAlpha != blendEquationAlpha
            ) {
            mCurrentBlending.mBlendEquation = blendEquation;
            mCurrentBlending.mBlendEquationAlpha = blendEquationAlpha;

            gl::blendEquationSeparate(toGL(blendEquation), toGL(blendEquationAlpha));
        }
    }

    void DriverState::setDepth(
            bool depthTest,
            bool depthWrite,
            CompareFunction depthFunction,
            double depthClearColor
    ) noexcept {
        if (!mCurrentDepth.mDepthTest && !depthTest) {
            return;
        }

        // depthTest决定了当前物体的绘制是否参与深度检测
        if (mCurrentDepth.mDepthTest != depthTest) {
            mCurrentDepth.mDepthTest = depthTest;
            if (!mCurrentDepth.mDepthTest) {
                gl::disable(GL_DEPTH_TEST);
            }
            else {
                gl::enable(GL_DEPTH_TEST);
            }
        }

        // depthWrite决定了当前物体如果通过了深度检测，是否用当前物体的fragment的深度更新depthBuffer
        if (mCurrentDepth.mDepthWrite != depthWrite) {
            mCurrentDepth.mDepthWrite = depthWrite;

            if (mCurrentDepth.mDepthWrite) {
                gl::depthMask(GL_TRUE);
            }
            else {
                gl::depthMask(GL_FALSE);
            }
        }

        if (mCurrentDepth.mDepthFunction != depthFunction) {
            mCurrentDepth.mDepthFunction = depthFunction;
            gl::depthFunc(toGL(depthFunction));
        }

        if (mCurrentDepth.mDepthClearColor != depthClearColor) {
            mCurrentDepth.mDepthClearColor = depthClearColor;
            gl::clearDepth(depthClearColor);
        }
    }

    void DriverState::bindFrameBuffer(const GLuint &frameBuffer) noexcept {
        if (mCurrentFrameBuffer != frameBuffer) {
            mCurrentFrameBuffer = frameBuffer;
            glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
        }
    }

    void DriverState::setClearColor(float r, float g, float b, float a) noexcept {
        glm::vec4 color(r, g, b, a);
        if (mCurrentColor.mClearColor != color) {
            mCurrentColor.mClearColor = color;
            gl::clearColor(r, g, b, a);
        }
    }
}