#version 410

layout (location = 0) in vec3 position;
layout (location = 1) in vec4 tangent;
layout (location = 2) in vec3 normal;
layout (location = 3) in vec2 uv;

// new variables to "unoptimize" the shader
out vec3 positionTemp;
out vec4 tangentTemp;
out vec3 normalTemp;
out vec2 uvTemp;
out mat4 mvpTemp;
out mat4 modelTemp;
out mat3 normalMatrixTemp;


out vec2 fragUV;
out vec3 fragWorldPos;
out vec3 fragNormal;
out mat3 TBN;

uniform mat4 mvp;
uniform mat4 modelView;
uniform mat4 model;
uniform mat3 normalMatrix;

// For level-of-detail transition fade
out vec3 fragCamSpacePos;

void main()
{
    gl_Position = mvp * vec4(position, 1.0);
    positionTemp = position;
    tangentTemp = tangent;
    normalTemp = normal;
    uvTemp = uv;

    mvpTemp = mvp;
    modelTemp = model;
    normalMatrixTemp = normalMatrix;

    fragWorldPos = (model * vec4(position, 1.0)).xyz;

    fragUV = vec2(uv.x, 1 - uv.y);
    
    // For level-of-detail transition fade
    fragCamSpacePos = (modelView * vec4(position, 1.0)).xyz;

    vec3 T = normalize(normalMatrix * tangent.xyz);
    vec3 B = normalize(normalMatrix * cross(tangent.xyz, normal));
    vec3 N = normalize(normalMatrix * normal);

    TBN = mat3(T, B, N);

    fragNormal = N;
}
