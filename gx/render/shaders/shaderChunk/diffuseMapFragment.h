#pragma once
#include "../../../global/base.h"

namespace gx {
    static const std::string diffuseMapFragment =
            "#ifdef HAS_DIFFUSE_MAP\n"\
            "   diffuseColor.rgb = texture(diffuseMap, fragUV).rgb;\n"\
            "#endif\n"\
            "\n";
}