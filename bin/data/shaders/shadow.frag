#version 410

in vec2 fragUV;
in vec3 fragWorldPos;
in vec3 fragNormal;

out vec4 outColor;

uniform sampler2D fboTexture;
uniform mat4 shadowMatrix;
uniform vec3 cameraPos;

uniform vec3 lightPosition;
uniform vec3 lightConeDirection;
uniform float lightCutoff;
uniform vec3 lightColor;
uniform vec3 ambientColor;

void main()
{
    // Project position into FBO clip space
    vec4 shadowCoords = shadowMatrix * vec4(fragWorldPos, 1.0);

    // Don't forget perspective divide
    vec3 texCoords = vec3(0.5) + 0.5 * shadowCoords.xyz / shadowCoords.w;

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
    //float nDotL = max(0.0, dot(wsNormal, lightDir)); 

    // how much light is effectively received by the surface
    vec3 irradiance = ambientColor + lightColor * falloff; // * nDotL; 



    outColor = vec4(vec3(0.25 + 
        0.5 * step(texCoords.z, texture(fboTexture, texCoords.xy).r)) * irradiance, 1.0);

        //outColor = texture(fboTexture, texCoords.xy);
}
