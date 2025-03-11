#include "animationAction.h"

namespace gx {

    AnimationAction::AnimationAction(const AnimationClip::Ptr &clip, const Object3D::Ptr &object) noexcept {
        mName = clip->mName;
        mClip = clip;
        mRoot = object;

        auto tracks = clip->mTracks;
        auto nTracks = tracks.size();

        // mInterpolants当中每一个插值器都对应了一个KeyframeTrack
        // mPropertyBindings当中的每一个都对应存储一个Interpolant（插值器）的ResultBuffer
        // mPropertyBindings当中的每一个都对应着当前动画的一根骨骼的属性(要么是position 要么是rotation 要么是scale)
        mInterpolants.resize(nTracks);
        mPropertyBindings.resize(nTracks);

        // make interpolants
        for (uint32_t i = 0; i < nTracks; ++i) {
            mInterpolants[i] = tracks[i]->makeInterpolant();
        }

        // make bindings
        for (int i = 0; i < nTracks; ++i) {
            mPropertyBindings[i] = PropertyBinding::create(mRoot, tracks[i]->mName, tracks[i]->getValueSize());
            mInterpolants[i]->setBuffer(static_cast<float*>(mPropertyBindings[i]->mBuffer));
        }
    }

    AnimationAction::~AnimationAction() noexcept {}

    void AnimationAction::play() noexcept {
        mRunning = true;
    }

    void AnimationAction::stop() noexcept {
        mRunning = false;
    }

    void AnimationAction::update(float deltaTime) noexcept {
        if (!mRunning) {
            return;
        }

        float duration = mClip->mDuration;
        float ticksPerSecond = mClip->mTicksPerSecond;

        mCurrentTime = fmod(mCurrentTime + deltaTime * ticksPerSecond * mSpeed, duration);

        // 对当前所有的KeyframeTracks做一次插值就三，计算的结果会存放在对应的PropertyBinding的mBuffer里面
        for (uint32_t i = 0; i < mInterpolants.size(); ++i) {
            mInterpolants[i]->evaluate(mCurrentTime);
        }

        // 此时每一个PropertyBinding里面的mBuffer都已经存放了当前时间最新的插值计算结果（position，quaternion，scale）
        // 循环遍历每一个PropertyBinding，使用apply函数，将它所对应的mNode（Bone）的对应属性，使用最新的mBuffer进行更新
        for (uint32_t i = 0; i < mPropertyBindings.size(); ++i) {
            mPropertyBindings[i]->apply();
        }
    }
}