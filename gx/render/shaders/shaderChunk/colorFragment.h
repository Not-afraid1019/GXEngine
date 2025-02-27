#pragma once
#include "../../../global/base.h"

namespace gx {
    static const std::string colorFragment =
            "#ifder HAS_COLOR\n"\
            "   diffuseColor.rgb *= fragColor;\n"\
            "#endif\n"\
            "\n";
}