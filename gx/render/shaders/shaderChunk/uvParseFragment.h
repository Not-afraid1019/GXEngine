#pragma once
#pragma once
#include "../../../global/base.h"

namespace gx {
    static const std::string uvParseFragment =
            "#ifdef HAS_UV\n"\
            "   in vec2 fragUV;\n"\
            "#endif\n"\
            "\n";
}