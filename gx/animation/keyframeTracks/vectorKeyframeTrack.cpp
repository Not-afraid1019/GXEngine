#include "vectorKeyframeTrack.h"

namespace gx {

    VectorKeyframeTrack::VectorKeyframeTrack(
            const std::string &name,
            const std::vector<float> &values,
            const std::vector<float> &times) noexcept : KeyframeTrack(name, values, times) {}

    VectorKeyframeTrack::~VectorKeyframeTrack() noexcept {}
}