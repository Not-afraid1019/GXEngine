#include "boxGeometry.h"

namespace gx {

    BoxGeometry::BoxGeometry(float size) noexcept {
        float halfSize = size / 2.0f;
        // 1 生成顶点数据
        std::vector<float> positions = {
                // Front face
                -halfSize, -halfSize,  halfSize, halfSize, -halfSize,  halfSize,halfSize,  halfSize,  halfSize,-halfSize,  halfSize,  halfSize,

                // Back face
                -halfSize, -halfSize, -halfSize, halfSize, -halfSize, -halfSize,halfSize, halfSize, -halfSize,-halfSize,  halfSize, -halfSize,

                // Top face
                -halfSize,  halfSize, -halfSize,halfSize,  halfSize, -halfSize,halfSize, halfSize,  halfSize,-halfSize,  halfSize,  halfSize, // 11

                // Bottom face
                -halfSize, -halfSize, -halfSize, halfSize, -halfSize, -halfSize, halfSize, -halfSize,  halfSize,-halfSize, -halfSize,  halfSize, // 15

                // Right face
                halfSize, -halfSize, -halfSize, halfSize,  halfSize, -halfSize,halfSize, halfSize,  halfSize,halfSize, -halfSize,  halfSize, // 19

                // Left face
                -halfSize, -halfSize, -halfSize,-halfSize,  halfSize, -halfSize,-halfSize, halfSize,  halfSize,-halfSize, -halfSize,  halfSize  // 23
        };

        mAttributes["positions"] = Attributef::create(positions, 3, BufferAllocType::StaticDrawBuffer);

        std::vector<float> uvs = {
                // Front face
                0.0, 0.0, 1.0, 0.0,1.0, 1.0,0.0, 1.0,  // 3

                // Back face
                0.0, 0.0,1.0, 0.0,1.0, 1.0,0.0, 1.0,  // 7

                // Top face
                0.0, 0.0,1.0, 0.0,1.0, 1.0, 0.0, 1.0,  // 11

                // Bottom face
                0.0, 0.0,1.0, 0.0,1.0, 1.0,0.0, 1.0,  // 15

                // Right face
                0.0, 0.0,1.0, 0.0,1.0, 1.0,0.0, 1.0,  // 19

                // Left face
                0.0, 0.0,1.0, 0.0,1.0, 1.0,0.0, 1.0   // 23
        };

        mAttributes["uvs"] = Attributef::create(uvs, 2, BufferAllocType::StaticDrawBuffer);

        std::vector<unsigned int> indices = {
                // Front face
                0, 2, 1,0, 3, 2,

                // Back face
                4, 5, 6,4, 6, 7,

                // Top face
                8, 9, 10,8, 10, 11,

                // Bottom face
                12, 14, 13,12, 15, 14,

                // Right face
                16, 18, 17,16, 19, 18,

                // Left face
                20, 21, 22,20, 22, 23
        };

        mIndexAttribute = Attributei::create(indices, 1, BufferAllocType::StaticDrawBuffer);

        std::vector<float> normals = {
                // Front
                0.0f, 0.0f, 1.0f,
                0.0f, 0.0f, 1.0f,
                0.0f, 0.0f, 1.0f,
                0.0f, 0.0f, 1.0f,
                // Back
                0.0f, 0.0f, -1.0f,
                0.0f, 0.0f, -1.0f,
                0.0f, 0.0f, -1.0f,
                0.0f, 0.0f, -1.0f,
                // Top face
                0.0f, 1.0f, 0.0f,
                0.0f, 1.0f, 0.0f,
                0.0f, 1.0f, 0.0f,
                0.0f, 1.0f, 0.0f,
                // Bottom face
                0.0f, -1.0f, 0.0f,
                0.0f, -1.0f, 0.0f,
                0.0f, -1.0f, 0.0f,
                0.0f, -1.0f, 0.0f,
                // Right face
                1.0f, 0.0f, 0.0f,
                1.0f, 0.0f, 0.0f,
                1.0f, 0.0f, 0.0f,
                1.0f, 0.0f, 0.0f,
                // Left face
                -1.0f, 0.0f, 0.0f,
                -1.0f, 0.0f, 0.0f,
                -1.0f, 0.0f, 0.0f,
                -1.0f, 0.0f, 0.0f,
        };

        mAttributes["normals"] = Attributef::create(normals, 3, BufferAllocType::StaticDrawBuffer);
    }

    BoxGeometry::~BoxGeometry() noexcept {}
}