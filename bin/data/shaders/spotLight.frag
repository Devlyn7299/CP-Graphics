#version 410

uniform vec3 lightPosition;
uniform vec3 lightConeDirection;
uniform float lightCutoff;
uniform vec3 lightColor;
uniform vec3 ambientColor;

uniform sampler2D diffuseTex;
uniform sampler2D normalTex;

in vec2 fragUV;
in vec3 fragWorldPos;
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

    // Calculate light direction
    vec3 toLight = lightPosition - fragWorldPos;
    vec3 lightDir = normalize(toLight);

    float falloff;
    float cosAngle = dot(lightConeDirection, -lightDir);

    // If the light direction is too far away from the light's cone direction
    // then no direct illumination
    // sign(x) = 1 if x > 0; 0 if x == 0; -1 if x < 0.
    // inverse square falloff
    falloff = max(0, sign(cosAngle - lightCutoff)) / dot(toLight, toLight);

    // Calculate cosine
    float nDotL = max(0.0, dot(wsNormal, lightDir)); 

    // how much light is effectively received by the surface
    vec3 irradiance = ambientColor + lightColor * falloff * nDotL; 

    // Gamma correction: physically-linear to perceptually-linear (encoding gamma)
    outColor = vec4(pow(diffuseColor.rgb * irradiance, vec3(1.0 / 2.2)), 1.0);
}
