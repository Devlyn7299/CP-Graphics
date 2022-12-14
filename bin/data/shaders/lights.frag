#version 410


uniform vec3 ambientColor;

uniform vec3 cameraPosition;

uniform vec3 dirLightDir;
uniform vec3 dirLightColor;

uniform vec3 pointLightPosition;
uniform vec3 pointLightColor;

uniform vec3 spotLightColor;
uniform vec3 spotLightConeDir;
uniform vec3 spotLightPos;
uniform float spotLightCutoff;

uniform sampler2D diffuseTex;
uniform sampler2D normalTex;

// The distance at which the robot should start to fade away
uniform float startFade;

// The distance at which the robot should be completely invisible
uniform float endFade;

// The color of the mesh
uniform vec3 meshColor;

// 1/gamma; used for gamma correction
uniform float gammaInv;

uniform bool hasTexture;
uniform bool usedForTerrain;

in vec2 fragUV;
in vec3 fragWorldPos;
in mat3 TBN;
in vec3 fragNormal;
// Input position in camera space (use for calculating fade-out).
in vec3 fragCamSpacePos;

out vec4 outColor;

void main()
{
    if (usedForTerrain)
    {
        // Re-normalize the normal vector after rasterization.
        vec3 normal = normalize(fragNormal);

        // Calculate diffuse lighting.
        float lightAmount = max(0.0, dot(normal, dirLightDir));
        vec3 fragLight = dirLightColor * lightAmount + ambientColor;
    
        // Calculate fade-out.
        float alpha = 1.0 - smoothstep(startFade, endFade, length(fragCamSpacePos));

        // Apply lighting and gamma correction.
        outColor = vec4(pow(meshColor * fragLight, vec3(gammaInv)), alpha);
    }

    else if (hasTexture)
    {
        vec3 diffuseColor = pow(texture(diffuseTex, fragUV).rgb, vec3(2.2));

        vec3 tsNormal = texture(normalTex, fragUV).xyz * 2 - 1;
        vec3 wsNormal = normalize(TBN * tsNormal);

        float dirNDotL = max(0.0, dot(wsNormal, dirLightDir)); // assuming lightDir pre-normalized

        vec3 toLight = spotLightPos - fragWorldPos;
        vec3 spotLightDir = normalize(toLight);
        float falloff;
        float cosAngle = dot(spotLightConeDir, -spotLightDir);
        falloff = max(0, sign(cosAngle - spotLightCutoff)) / dot(toLight, toLight);
        float spotNDotL = max(0.0, dot(wsNormal, spotLightDir));

        vec3 irradiance = ambientColor + dirLightColor * dirNDotL + spotLightColor * spotNDotL;

        outColor = vec4(pow(diffuseColor.rgb * irradiance, vec3(1.0 / 2.2)), 1.0);
    }


    else {
        vec3 normal = normalize(fragNormal);
        float dirNDotL = max(0.0, dot(normal, dirLightDir)); // assuming lightDir pre-normalized

        vec3 toLight = spotLightPos - fragWorldPos;
        vec3 spotLightDir = normalize(toLight);
        float falloff;
        float cosAngle = dot(spotLightConeDir, -spotLightDir);
        falloff = max(0, sign(cosAngle - spotLightCutoff)) / dot(toLight, toLight);
        float spotNDotL = max(0.0, dot(normal, spotLightDir));

        vec3 irradiance = ambientColor + dirLightColor * dirNDotL + spotLightColor * spotNDotL;

        outColor = vec4(pow(vec3(0.1, 0.1, 0.1) * irradiance, vec3(1.0 / 2.2)), 1.0);
    }
}
