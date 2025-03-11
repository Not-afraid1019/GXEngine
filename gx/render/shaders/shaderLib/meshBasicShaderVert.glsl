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

void main() {
    #ifdef HAS_NORMAL
    vec3 objectNormal =  vec3(normal);
    #ifdef USE_TANGENT
    vec3 objectTangent = vec3(tangent);
    vec3 objectBitangent = vec3(bitangent);
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


    vec3 transformed = vec3(position);

    vec4 mvPosition = vec4(transformed, 1.0);
    mvPosition = modelViewMatrix * mvPosition;
    gl_Position = projectionMatrix * mvPosition;

    #ifdef HAS_NORMAL
    fragNormal = normalize(transformedNormal);
    #ifdef USE_TANGENT
    fragTangent = normalize(transformedTangent);
    fragBitangent = normalize(transformedBitangent);
    #endif
    #endif


    #ifdef HAS_COLOR
    fragColor = color;
    #endif


    #ifdef HAS_UV
    fragUV = uv;
    #endif


}