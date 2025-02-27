#pragma once
#include "../../../global/base.h"

namespace gx {

    static const std::string envMapCommonParseFragment =
            "#ifdef USE_ENVMAP\n"\
            "   uniform samplerCube envMap;\n"\
            "#endif\n"\
            "\n";
}