#pragma once
#include "../../../global/base.h"

namespace gx {
    static const std::string lightsPhongMaterial =
            "BlinnPhongMaterial material;\n"\
            "material.diffuseColor = diffuseColor.rgb;\n"\
            "material.specularShininess = shininess;\n"\
            "material.specularStrength = specularStrength;\n"\
            "\n";
}