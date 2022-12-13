#version 410

// The direction of the sun
uniform vec3 lightDir;

// The color of the sun
uniform vec3 lightColor;

// The ambient light color.
uniform vec3 ambientColor;

uniform vec3 cameraPosition;

uniform sampler2D diffuseTex;
uniform sampler2D normalTex;
uniform sampler2D specularTex;

uniform float shininess;

uniform samplerCube envMap;

uniform int reflection;

// The color of the robot
uniform vec3 meshColor;

// 1/gamma; used for gamma correction
uniform float gammaInv;

// The distance at which the robot should start to fade away
uniform float startFade;

// The distance at which the robot should be completely invisible
uniform float endFade;

// Input surface normal
in vec3 fragNormal;

// Input position in camera space (use for calculating fade-out).
in vec3 fragCamSpacePos;

in vec2 fragUV;
in vec3 fragWorldPos;
in mat3 TBN;

// Output color
out vec4 outColor;

void main()
{
    vec3 diffuseColor = pow(texture(diffuseTex, fragUV).rgb, vec3(2.2));

    // Re-normalize the normal vector after rasterization.
    vec3 tsNormal = texture(normalTex, fragUV).xyz * 2 - 1;
    vec3 wsNormal = normalize(TBN * tsNormal);
    // Calculate diffuse lighting.
    
    float nDotL = max(0.0, dot(wsNormal, lightDir));

    vec3 irradiance = ambientColor + lightColor * nDotL;

    outColor = vec4(pow(diffuseColor.rgb * irradiance, vec3(1.0 / 2.2)), 1.0);

}
