#version 410

in vec2 fragUV;
in vec3 fragWorldPos;
in vec3 fragNormal;

in mat3 TBN;


out vec4 outColor;

uniform sampler2D fboTexture;
uniform mat4 shadowMatrix;
uniform mat4 shadowMatrix2;

uniform float lightType;

uniform vec3 cameraPos;

uniform sampler2D diffuseTex;
uniform sampler2D normalTex;
uniform sampler2D specularTex;

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


uniform float shininess;

uniform samplerCube envMap;

uniform int reflection;


// The direction of the sun
uniform vec3 lightDir;

// The color of the robot
uniform vec3 meshColor;

// 1/gamma; used for gamma correction
uniform float gammaInv;

// The distance at which the robot should start to fade away
uniform float startFade;

// The distance at which the robot should be completely invisible
uniform float endFade;

// Input position in camera space (use for calculating fade-out).
in vec3 fragCamSpacePos;

//uniform vec3 lightPosition;
//uniform vec3 lightConeDirection;
//uniform float lightCutoff;
//uniform vec3 lightColor;


void main()
{
    // Project position into FBO clip space
    vec4 shadowCoords = shadowMatrix * vec4(fragWorldPos, 1.0);
    //vec4 shadowCoords2 = shadowMatrix2 * vec4(fragWorldPos, 1.0);

    // Don't forget perspective divide
    vec3 texCoords = vec3(0.5) + 0.5 * shadowCoords.xyz / shadowCoords.w;
    //vec3 texCoords2 = vec3(0.5) + 0.5 * shadowCoords2.xyz / shadowCoords2.w;

    // Normal mapping
    vec3 normal = normalize(fragNormal);


    // Calculate diffuse lighting.
    float lightAmount = max(0.0, dot(normal, lightDir));
    vec3 fragLight = dirLightColor * lightAmount + ambientColor;
    
    // Calculate fade-out.
    float alpha = 1.0 - smoothstep(startFade, endFade, length(fragCamSpacePos));




 // Calculate light direction
    //vec3 toSpotLight = spotLightPos - fragWorldPos;
    //vec3 spotLightDir = normalize(toSpotLight);


    //float spotLightFalloff;
    //float cosAngle = dot(spotLightConeDir, -spotLightDir);


    // inverse square falloff
    //spotLightFalloff = max(0, sign(cosAngle - spotLightCutoff)) / dot(toSpotLight, toSpotLight);

    // Calculate cosine
    //float spotNDotL = max(0.0, dot(normal, spotLightDir)); 

    // Directional light stuff
    float dirNDotL = max(0.0, dot(normal, dirLightDir));

    // how much light is effectively received by the surface
    vec3 irradiance;
    //if (lightType == 1)
    irradiance = ambientColor
        //+ spotLightColor * spotNDotL * spotLightFalloff * (step(texCoords2.z, texture(fboTexture, texCoords2.xy).r))
        + dirLightColor * dirNDotL * (step(texCoords.z, texture(fboTexture, texCoords.xy).r)); 
    
    //if (lightType == 2)
    //irradiance = ambientColor
    //    + spotLightColor * spotNDotL * spotLightFalloff * (step(texCoords2.z, texture(fboTexture, texCoords2.xy).r));
        //+ dirLightColor * dirNDotL * (step(texCoords.z, texture(fboTexture, texCoords.xy).r)); 

    //for (int i = 0; i < 10000; i++)
    //{
    //    irradiance += i * 1.0e-12;
    //}
    outColor = vec4(pow(meshColor * irradiance, vec3(gammaInv)), alpha);

    //outColor = vec4(irradiance, 1.0);

        //outColor = texture(fboTexture, texCoords.xy);
        //outColor = vec4(texCoords.xy, 0, 1);
}

