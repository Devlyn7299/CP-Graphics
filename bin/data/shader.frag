#version 410

//in vec2 fragUV;
in vec3 fragNormal;
in vec4 cameraPos;

out vec4 outColor;

uniform vec4 addedColor;
uniform vec3 lightDir;
uniform vec3 lightColor;
uniform vec3 meshColor;
uniform vec3 ambientColor;
uniform float nearPlane;
uniform float midPlane;
uniform bool needsSmoothstep;

void main()
{
    float camDistance = length(cameraPos);
    float smoothTest = 1;
    if (needsSmoothstep)
        smoothTest = 1 - smoothstep(nearPlane, midPlane, camDistance);
    vec3 normal = normalize(fragNormal);

    float nDotL = max(0, dot(normal, lightDir));
    vec3 irradiance = ambientColor + lightColor * nDotL; // how much light the surface effictively receives

    vec3 linearColor = meshColor * irradiance;

    outColor = vec4(pow(linearColor, vec3(1.0 / 2.2)), smoothTest);
    //outColor = vec4(1, 0, 0, 1);
}
