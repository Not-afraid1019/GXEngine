#pragma once
#include "../../../global/base.h"

namespace gx {

    static const std::string specularMapParseFragment =
            "#ifdef USE_SPECULARMAP\n"\
            "   uniform sampler2D specularMap;\n"\
            "#endif\n"\
            "\n";
}