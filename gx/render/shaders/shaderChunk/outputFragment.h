#pragma once
#include "../../../global/base.h"

namespace gx {
    static const std::string outputFragment =
            "fragmentColor = vec4(outgoingLight, diffuseColor.a);\n"\
            "\n";
}