#pragma once
#include "../../../global/base.h"

namespace gx {
    static const std::string uvParseVertex =
            "#ifdef HAS_UV\n"\
            "   layout(location = UV_LOCATION) in vec2 uv;\n"\
            "   out vec2 fragUV;\n"\
            "#endif\n"\
            "\n";
}