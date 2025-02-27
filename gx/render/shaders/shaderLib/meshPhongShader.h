#pragma once
#include "../../../global/base.h"
#include "../shaderChunk/shaderChunk.h"

namespace gx {

    namespace meshPhong {

        // fs当中所有光照方向、位置等几何信息都是在摄像机坐标系下进行
        // 明确每个shader的输入/输出
        static const std::string vertex =
                // 输送给fs的变量，表示摄像机坐标系下的顶点位置，在fs里面每一个fragment都会得到他自己的摄像机坐标系下的位置
                "out vec3 viewPosition;\n" +
                common +

                // 各种attribute的输入承接
                positionParseVertex +
                normalParseVertex +
                colorParseVertex +
                uvParseVertex +
                // 各类矩阵的集合
                uniformMatricesVertex;

                // 阴影与骨骼相关


//        static const std::string fragment = "";

    }
}