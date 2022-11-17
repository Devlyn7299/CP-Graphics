#version 410

uniform vec3 lightDir;
uniform vec3 lightColor;
uniform vec3 ambientColor;

uniform vec3 dirLightDir;
uniform vec3 dirLightColor;

uniform vec3 spotLightColor;
uniform vec3 spotLightConeDir;
uniform vec3 spotLightPos;
uniform float spotLightCutoff;

uniform sampler2D diffuseTex;
uniform sampler2D normalTex;

uniform samplerCube envMap;
in vec2 fragUV;
in mat3 TBN;
in vec3 fragWorldPos;


out vec4 outColor;

void main()
{
    // Extacting color from texture
    // Gamma correcion: perceptually linear to physically linear (decoding gamma)
    vec3 diffuseColor = pow(texture(diffuseTex, fragUV).rgb, vec3(2.2));
    
    // Normal mapping
    vec3 tsNormal = texture(normalTex, fragUV).xyz * 2 - 1;
    vec3 wsNormal = normalize(TBN * tsNormal);

    float dirNDotL = max(0.0, dot(wsNormal, dirLightDir)); // assuming lightDir pre-normalized

    // Calculate spot lighting
    vec3 toSpotLight = spotLightPos - fragWorldPos;
    vec3 spotLightDir = normalize(toSpotLight);
    float cosAngle = dot(spotLightDir, -spotLightConeDir);
    float spotNDotL = max(0, dot(wsNormal, spotLightDir));
    vec3 spotLight;
    if (cosAngle > spotLightCutoff) 
    {
        spotLight = spotLightColor * spotNDotL;
    }



    vec3 envIrradiance = pow(textureLod(envMap, wsNormal, 99).rgb, vec3(2.2));

    // how much light is effectively received by the surface
    vec3 irradiance = ambientColor + envIrradiance + dirLightColor * dirNDotL;

    if (cosAngle > spotLightCutoff)
        irradiance = ambientColor + spotLight + envIrradiance + dirLightColor * dirNDotL;


    // Gamma correction: physically-linear to perceptually-linear (encoding gamma)
    outColor = vec4(pow(diffuseColor.rgb * irradiance, vec3(1.0 / 2.2)), 1.0);
}
