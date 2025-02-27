#pragma once
#include "../global/base.h"
#include "texture.h"

namespace gx {

    class DepthTexture : public Texture {
    public:
        using Ptr = std::shared_ptr<DepthTexture>;
        
    };
}