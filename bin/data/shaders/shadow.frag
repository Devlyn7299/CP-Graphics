#version 410

in vec2 fragUV;
in vec3 fragWorldPos;
in vec3 fragNormal;

out vec4 outColor;

uniform sampler2D fboTexture;
uniform mat4 shadowMatrix;
uniform vec3 cameraPos;

void main()
{
    // Project position into FBO clip space
    vec4 shadowCoords = shadowMatrix * vec4(fragWorldPos, 1.0);

    // Don't forget perspective divide
    vec3 texCoords = vec3(0.5) + 0.5 * shadowCoords.xyz / shadowCoords.w;

    outColor = vec4(vec3(0.1, 0.9, 0.1) * vec3(0.25 + 
        0.5 * step(texCoords.z, texture(fboTexture, texCoords.xy).r)), 1.0);

        //outColor = texture(fboTexture, texCoords.xy);
}
