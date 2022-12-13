#version 410

layout (location = 0) in vec3 pos;
layout (location = 1) in vec4 tangent;
layout (location = 2) in vec3 normal;
layout (location = 3) in vec2 uv;

// Needs both model-view-projection and model-view; model-view is used for level-of-detail transition fade.
uniform mat4 mvp;
uniform mat4 modelView;
uniform mat3 normalMatrix;
uniform mat4 model;

// Pass-through normal and UV.
out vec2 fragUV;
out vec3 fragWorldPos;
out vec3 fragNormal;
out mat3 TBN;

// For level-of-detail transition fade
out vec3 fragCamSpacePos;

void main()
{
    // Standard model-view-projection transformation.
    gl_Position = mvp * vec4(pos, 1.0);
    fragWorldPos = (model * vec4(pos, 1.0)).xyz;

    // Pass-through normal and UV.
    fragNormal = normalize(normalMatrix * normal);
    fragUV = vec2(uv[0], 1 - uv[1]);

    // For level-of-detail transition fade
    fragCamSpacePos = (modelView * vec4(pos, 1.0)).xyz;

    vec3 T = normalize(normalMatrix * tangent.xyz);
    vec3 B = normalize(normalMatrix * cross(tangent.xyz, normal));
    vec3 N = normalize(normalMatrix * normal);

    TBN = mat3(T, B, N);

    fragNormal = N;
}
