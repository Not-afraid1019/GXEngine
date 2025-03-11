out vec3 viewPosition;

#define PI 3.141592653589793
#define PI2 6.283185307179586
#define PI_HALF 1.5707963267948966

#ifndef saturate
#define saturate(a) clamp(a, 0.0, 1.0)
#endif


// 入射光：不管是点光源、聚光灯、平行光，多种光源计算光线的方式不同
// 但是最终得到的结果就是Color和Direction
struct IncidentLight {
    vec3 color;
    vec3 direction;
};


// 反射光：大体上分为直接光照与间接光照
struct ReflectedLight {
    vec3 directDiffuse;
    vec3 directSpecular;
    vec3 indirectDiffuse;
    vec3 indirectSpecular;
};


// 几何体描述
struct GeometricContext {
    vec3 position;
    vec3 normal;
    vec3 viewDir;
};

layout(location = POSITION_LOCATION) in vec3 position;

#ifdef HAS_NORMAL
layout(location = NORMAL_LOCATION) in vec3 normal;
out vec3 fragNormal;
#ifdef USE_TANGENT
layout(location = TANGENT_LOCATION) in vec3 normal;
out vec3 fragTangent;

layout(location = BITANGENT_B_LOCATION) in vec3 bitangent;
out vec3 fragBitangent;
#endif
#endif

#ifder HAS_COLOR
layout(location = COLOR_LOCATION) in vec3 color;
out vec3 fragColor;
#endif

#ifdef HAS_UV
layout(location = UV_LOCATION) in vec2 uv;
out vec2 fragUV;
#endif

uniform mat4 modelViewMatrix;
uniform mat3 normalMatrix;
uniform mat4 projectionMatrix;
uniform mat4 modelMatrix;

#ifdef USE_SHADOWMAP
#if NUM_DIR_LIGHT_SHADOWS > 0
uniform mat4 directionalShadowMatrix[NUM_DIR_LIGHT_SHADOWS];
out vec4 directionalShadowCoords[NUM_DIR_LIGHT_SHADOWS];

struct DirectionalLightShadow {
    float shadowRadius;
    float shadowBias;
    float shadowMapSize;
};

uniform DirectionalLightShadow directionalLightShadows[NUM_DIR_LIGHT_SHADOWS];
#endif
#endif

#ifdef USE_SKINNING
layout(location = SKINNING_INDICES_LOCATION) in vec4 skinIndex;
layout(location = SKINNING_INDICES_LOCATION) in vec4 skinWeight;

uniform mat4 boneMatrices[MAX_BONES];
mat4 getBoneMatrix(const in float i) {
    return boneMatrices[int(i)];
}
#endif

void main() {
    #ifdef HAS_NORMAL
    vec3 objectNormal =  vec3(normal);
    #ifdef USE_TANGENT
    vec3 objectTangent = vec3(tangent);
    vec3 objectBitangent = vec3(bitangent);
    #endif
    #endif

    #ifdef USE_SKINNING
    mat4 boneMatX = getBoneMatrix(skinIndex.x);
    mat4 boneMatY = getBoneMatrix(skinIndex.y);
    mat4 boneMatZ = getBoneMatrix(skinIndex.z);
    mat4 boneMatW = getBoneMatrix(skinIndex.w);
    #endif

    #ifdef USE_SKINNING
    mat4 skinMatrix = mat4(0.0);
    skinMatrix += boneMatX * skinWeight.x;
    skinMatrix += boneMatY * skinWeight.y;
    skinMatrix += boneMatZ * skinWeight.z;
    skinMatrix += boneMatW * skinWeight.w;

    objectNormal = vec4(skinMatrix * vec4(objectNormal, 0.0)).xyz;
    #ifdef USE_TANGENT
    objectTangent = vec4(skinMatrix * vec4(objectTangent, 0.0)).xyz;
    objectBitangent = vec4(skinMatrix * vec4(objectBitangent, 0.0)).xyz;
    #endif
    #endif

    #ifdef HAS_NORMAL
    vec3 transformedNormal = normalMatrix * objectNormal;

    #ifdef USE_TANGENT
    // because tangent is the base vector
    vec3 transformedTangent = (modelViewMatrix * vec4(objectTangent, 0.0)).xyz;
    vec3 transformedBitangent = (modelViewMatrix * vec4(objectBitangent, 0.0)).xyz;
    #endif
    #endif

    #ifdef HAS_NORMAL
    fragNormal = normalize(transformedNormal);
    #ifdef USE_TANGENT
    fragTangent = normalize(transformedTangent);
    fragBitangent = normalize(transformedBitangent);
    #endif
    #endif

    vec3 transformed = vec3(position);

    #ifdef USE_SKINNING
    vec4 skinVertex = vec4(transformed, 1.0);
    vec4 skinned = vec4(0.0);
    skinned += boneMatX * skinVertex * skinWeight.x;
    skinned += boneMatY * skinVertex * skinWeight.y;
    skinned += boneMatZ * skinVertex * skinWeight.z;
    skinned += boneMatW * skinVertex * skinWeight.w;

    transformed = skinned.xyz;
    #endif

    vec4 mvPosition = vec4(transformed, 1.0);
    mvPosition = modelViewMatrix * mvPosition;
    gl_Position = projectionMatrix * mvPosition;

    #ifdef HAS_COLOR
    fragColor = color;
    #endif

    #ifdef HAS_UV
    fragUV = uv;
    #endif

    viewPosition = mvPosition.xyz;

    #if defined(USE_SHADOWMAP) || defined(USE_ENVMAP)
    vec4 worldPosition = vec4(transformed, 1.0);
    worldPosition = modelMatrix * worldPosition;
    #endif

    #ifdef USE_SHADOWMAP
    #if NUM_DIR_LIGHT_SHADOWS > 0
    vec4 shadowWorldPosition;
    #endif

    #if NUM_DIR_LIGHT_SHADOWS > 0
    for (int i = 0; i < NUM_DIR_LIGHT_SHADOWS; i++) {
        shadowWorldPosition = worldPosition;
        directionalShadowCoords[i] = directionalShadowMatrix[i] * shadowWorldPosition;
    }
    #endif

    #endif
}