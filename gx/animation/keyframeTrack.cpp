#include "keyframeTrack.h"
#include "../math/interpolants/quaternionInterpolant.h"
#include "../math/interpolants/linearInterpolant.h"

namespace gx {

    KeyframeTrack::KeyframeTrack(
            const std::string &name,
            const std::vector<float> &values,
            const std::vector<float> &times) noexcept {
        mName = name;
        mValues = values;
        mTimes = times;
    }

    KeyframeTrack::~KeyframeTrack() noexcept {}

    Interpolant::Ptr KeyframeTrack::makeInterpolant() noexcept {
        return LinearInterpolant::create(mTimes, mValues, getValueSize());
    }

    // 获取对应Interpolant的sampleSize，比如position变换，返回3，比如四元数变换，返回4
    uint32_t KeyframeTrack::getValueSize() noexcept {
        return mValues.size() / mTimes.size();
    }
}