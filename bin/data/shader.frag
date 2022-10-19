#version 410

in vec2 fragUV;
in vec3 fragNormal;

out vec4 outColor;

void main()
{
    vec3 normal = normalize(fragNormal);
    outColor = vec4(normal * 0.5 + vec3(0.5), 1.0);
    //outColor = vec4(1, 0, 0, 1);
}
