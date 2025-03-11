#pragma once
#include "../global/base.h"
#include "../core/geometry.h"

namespace gx {

    class BoxGeometry : public Geometry {
    public:
        using Ptr = std::shared_ptr<BoxGeometry>;
        static Ptr create(float size) {
            return std::make_shared<BoxGeometry>(size);
        }

        BoxGeometry(float size) noexcept;

        ~BoxGeometry() noexcept;
    };
}