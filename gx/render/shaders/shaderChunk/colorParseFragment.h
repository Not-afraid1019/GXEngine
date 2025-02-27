#pragma once
#include "../../../global/base.h"

namespace gx {
    static const std::string colorParseFragment =
            "#ifdef HAS_COLOR\n"\
            "   in vec3 fragColor;\n"\
            "#endif\n"\
            "\n";
}