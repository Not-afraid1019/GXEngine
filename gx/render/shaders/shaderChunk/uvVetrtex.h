#pragma once
#include "../../../global/base.h"

namespace gx {
    static const std::string uvVertex =
            "#ifdef HAS_UV\n"\
            "   fragUV = uv;\n"\
            "#endif\n"\
            "\n";
}