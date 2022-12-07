#version 410

in vec2 fragUV;
in vec3 fragWorldPos;
in vec3 fragNormal;

out vec4 outColor;

uniform sampler2D fboTexture;
uniform mat4 reflectionMatrix;
uniform vec3 cameraPos;

void main()
{
    // calculate view vector (direction from surface to camera)
    vec3 view = normalize(cameraPos - fragWorldPos);

    // Calculating reflected light direction
    vec3 envLightDir = reflect(-view, fragNormal); 

    // Project reflection direction into FBO clip space
    vec4 reflPos = reflectionMatrix * vec4(envLightDir, 1.0);

    // Don't forget perspective divide
    vec2 texCoords = vec2(0.5) + 0.5 * reflPos.xy / reflPos.w;

    outColor = texture(fboTexture, texCoords);
}
