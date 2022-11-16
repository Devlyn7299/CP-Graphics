#version 410

uniform vec3 lightDir;
uniform vec3 lightColor;
uniform vec3 ambientColor;

uniform sampler2D diffuseTex;
uniform sampler2D normalTex;

uniform samplerCube envMap;
in vec2 fragUV;
in mat3 TBN;

out vec4 outColor;

void main()
{
    // Extacting color from texture
    // Gamma correcion: perceptually linear to physically linear (decoding gamma)
    vec3 diffuseColor = pow(texture(diffuseTex, fragUV).rgb, vec3(2.2));
    
    // Normal mapping
    vec3 tsNormal = texture(normalTex, fragUV).xyz * 2 - 1;
    vec3 wsNormal = normalize(TBN * tsNormal);

    float nDotL = max(0.0, dot(wsNormal, lightDir)); // assuming lightDir pre-normalized

    vec3 envIrradiance = pow(textureLod(envMap, wsNormal, 99).rgb, vec3(2.2));

    // how much light is effectively received by the surface
    vec3 irradiance = ambientColor + envIrradiance + lightColor * nDotL;

    // Gamma correction: physically-linear to perceptually-linear (encoding gamma)
    outColor = vec4(pow(diffuseColor.rgb * irradiance, vec3(1.0 / 2.2)), 1.0);
}
