in vec3 viewPosition;
uniform float shininess;
uniform float opacity;

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


const float PackUpscale = 256. / 255.;// fraction ->0.. 1 (including 1)
const float UnpackDownscale = 255. / 256.;// 0.. 1 ->fraction (excluding 1)
const vec3 PackFactors = vec3(256. * 256. * 256., 256. * 256, 256.);
const vec4 UnpackFactors = UnpackDownscale / vec4(PackFactors, 1.);

const float ShiftRight8 = 1. / 256.;

vec4 packDepthToRGBA(const in float v) {
    vec4 r = vec4(fract(v * PackFactors), v);
    r.yzw -= r.xyz * ShiftRight8;// tidy overflow
    return r * PackUpscale;
}

float unpackRGBAToDepth(const in vec4 v) {
    return dot(v, UnpackFactors);
}

#ifdef HAS_NORMAL
in vec3 fragNormal;
#ifdef USE_TANGENT
in vec3 fragTangent;
in vec3 fragBitangent;
#endif
#endif


#ifdef USE_NORMALMAP
uniform sampler2D normalMap;
#endif


#ifdef USE_SPECULARMAP
uniform sampler2D specularMap;
#endif


#ifdef HAS_COLOR
in vec3 fragColor;
#endif


#ifdef HAS_UV
in vec2 fragUV;
#endif


#ifdef HAS_DIFFUSE_MAP
uniform sampler2D diffuseMap;
#endif


uniform vec3 ambientLightColor;

#if NUM_DIR_LIGHTS > 0
struct DirectionalLight {
    vec3 direction;
    vec3 color;
};

uniform DirectionalLight directionalLights[NUM_DIR_LIGHTS];

void getDirectionalLightInfo(const in DirectionalLight directionalLight, const in GeometricContext geometry, out IncidentLight light) {
    light.color = directionalLight.color;
    light.direction = directionalLight.direction;
}
#endif



struct BlinnPhongMaterial {
    vec3 diffuseColor;
    float specularShininess;
    float specularStrength;
};

void RE_Direct_BlinnPhong(const in IncidentLight directLight, const in Geometric geometry, const in BlinnPhongMaterial material, inout ReflectedLight reflectedLight) {
    // diffuse computing
    float dotNL = saturate(dot(geometry.normal, -directLight.direction));
    reflectedLight.directDiffuse += material.diffuseColor * (directLight.color * ambientLightColor);

    // specular computing
    vec3 halfWayDir = normalize(-directLight.direction + -geometry.viewDir);
    float dotHN = max(dot(geometry.normal, halfWayDir), 0.0);
    reflectedLight.directSpecular += pow(dotNH, material.specularShininess) * directLight.color * material.specularStrength;
}

#define RE_Direct RE_Direct_BlinnPhong


#ifdef USE_SHADOWMAP
#if NUM_DIR_LIGHT_SHADOWS > 0
uniform sampler2D directionalShadowMap[NUM_DIR_LIGHT_SHADOWS];
in vec4 directionalShadowCoords[NUM_DIR_LIGHT_SHADOWS];

struct DirectionalLightShadow {
    float shadowRadius;
    float shadowBias;
    float shadowMapSize;
};

uniform DirectionalLightShadow directionalLightShadows[NUM_DIR_LIGHT_SHADOWS];
#endif
// return 1 if texture value is bigger than compare
float texture2DCompare(sampler2D depths, vec2 uv, float compare) {
    return step(compare, unpackRGBAToDepth(texture2d(depths, uv)));
}

float getShadow(sampler2D shadowMap, vec2 shadowMapSize, float shadowBias, float shadowRadius, vec4 shadowCoord) {
    float shadow = 1.0;

    shadowCoord.xyz /= shadowCoord.w;
    shadowCoord.z += shadowBias;

    bvec4 inFrustumXYVec = bvec4(shadowCoord.x >= 0.0, shadowCoord.x <= 1.0, shadowCoord.y >= 0.0, shadowCoord.y <=1.0);
    bool inFrustumXY = all(inFrustumXYVec);

    bvec2 inFrustumZVec = bvec2(inFrustumXY, shadowCoord.z <= 1.0);

    bool inFrustum = all(inFrustumZVec);
    if (inFrustum) {
        vec2 texelSize = vec2(1.0) / shadowMapSize;
        float dx0 = -texelSize.x * shadowRadius;
        float dy0 = -texelSize.y * shadowRadius;
        float dx1 = texelSize.x * shadowRadius;
        float dy1 = texelSize.y * shadowRadius;
        float dx2 = dx0 / 2.0;
        float dy2 = dy0 / 2.0;
        float dx3 = dx1 / 2.0;
        float dy3 = dy1 / 2.0;

        shadow = (
        texture2DCompare(shadowMap, shadowCoord.xy + vec2(dx0, dy0), shadowCoord.z) +
        texture2DCompare(shadowMap, shadowCoord.xy + vec2(0.0, dy0), shadowCoord.z) +
        texture2DCompare(shadowMap, shadowCoord.xy + vec2(dx1, dy0), shadowCoord.z) +
        texture2DCompare(shadowMap, shadowCoord.xy + vec2(dx2, dy2), shadowCoord.z) +
        texture2DCompare(shadowMap, shadowCoord.xy + vec2(0.0, dy2), shadowCoord.z) +
        texture2DCompare(shadowMap, shadowCoord.xy + vec2(dx3, dy2), shadowCoord.z) +
        texture2DCompare(shadowMap, shadowCoord.xy + vec2(dx0, 0.0), shadowCoord.z) +
        texture2DCompare(shadowMap, shadowCoord.xy + vec2(dx2, 0.0), shadowCoord.z) +
        texture2DCompare(shadowMap, shadowCoord.xy, shadowCoord.z) +
        texture2DCompare(shadowMap, shadowCoord.xy + vec2(dx3, 0.0), shadowCoord.z) +
        texture2DCompare(shadowMap, shadowCoord.xy + vec2(dx1, 0.0), shadowCoord.z) +
        texture2DCompare(shadowMap, shadowCoord.xy + vec2(dx2, dy3), shadowCoord.z) +
        texture2DCompare(shadowMap, shadowCoord.xy + vec2(0.0, dy3), shadowCoord.z) +
        texture2DCompare(shadowMap, shadowCoord.xy + vec2(dx3, dy3), shadowCoord.z) +
        texture2DCompare(shadowMap, shadowCoord.xy + vec2(dx0, dy0), shadowCoord.z) +
        texture2DCompare(shadowMap, shadowCoord.xy + vec2(0.0, dy0), shadowCoord.z) +
        texture2DCompare(shadowMap, shadowCoord.xy + vec2(dx1, dy0), shadowCoord.z)
        ) * (1.0 / 17.0);
    }
return shadow;
}
#endif


uniform mat4 modelViewMatrix;

out vec4 fragmentColor;

void main() {
    vec4 diffuseColor = vec4(vec3(1.0), opacity);

    #ifdef HAS_NORMAL
    vec3 normal = normalize(fragNormal);
    #ifdef USE_TANGENT
    vec3 tangent = normalize(fragTangent);
    vec3 bitangent = normalize(fragBitangent);
    mat3 TBN = mat3(tangent, bitangent, normal);
    #endif
    #endif


    #ifdef USE_NORMALMAP
    normal = texture2D(normalMap, fragUV).xyz * 2.0 - 1.0;
    normal = normalize(TBN * normal);
    #endif


    #ifdef HAS_DIFFUSE_MAP
    diffuseColor.rgb = texture(diffuseMap, fragUV).rgb;
    #endif


    #ifder HAS_COLOR
    diffuseColor.rgb *= fragColor;
    #endif


    float specularStrength = 1.0;
    #ifdef USE_SPECULARMAP
    specularStrength = texture2D(specularMap, fragUV).r;
    #endif


    ReflectedLight reflectedLight = ReflectedLight(vec3(0.0), vec3(0.0), vec3(0.0), vec3(0.0));

    BlinnPhongMaterial material;
    material.diffuseColor = diffuseColor.rgb;
    material.specularShininess = shininess;
    material.specularStrength = specularStrength;


    GeometricContext geometry;

    geometry.position = viewPosition;
    geometry.normal = normal;
    geometry.viewDir = normalize(viewPosition);

    IncidentLight directLight;

    #if (NUM_DIR_LIGHTS > 0) && defined(RE_Direct)
    DirectionalLight directionalLight;
    #if defined(USE_SHADOWMAP) && NUM_DIR_LIGHTS_SHADOWS > 0
    DirectionalLightShadow directionalLightShadow;
    #endif
    for (int i = 0; i < NUM_DIR_LIGHTS; i++) {
        directionalLight = directionalLights[i];
        getDirectionalLightInfo(directionalLight, geometry, directLight);

        #if defined(USE_SHADOWMAP) && NUM_DIR_LIGHT_SHADOWS > 0
        if (i < NUM_DIR_LIGHT_SHADOWS) {
            directionalLightShadow = directionalLightShadows[i];
            directLight.color *= getShadow(directionalShadowMap[i], directionalLightShadow.shadowMapSize, directionalLightShadow.shadowBias, directionalLightShadow.shadowRadius, directionalShadowCoords[i]);
        }
        #endif
        RE_Direct(directLight, geometry, material, reflectedLight);
    }
    #endif


    vec3 outgoingLight = reflectedLight.directDiffuse + reflectedLight.directSpecular + reflectedLight.indirectDiffuse + reflectedLight.indirectSpecular;


    fragmentColor = vec4(outgoingLight, diffuseColor.a);

}