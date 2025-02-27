#pragma once
#include "../../../global/base.h"

namespace gx {
    static const std::string colorParseVertex =
            "#ifder HAS_COLOR\n"\
            "   layout(location = COLOR_LOCATION) in vec3 color;\n"\
            "   out vec3 fragColor;\n"\
            "#endif\n"\
            "\n";
}