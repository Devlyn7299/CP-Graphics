#version 410

in vec3 fragNormal;
in vec4 cameraPos;
in vec2 fragUV;

out vec4 outColor;

// Pulling in multiple variables for both lighting and smoothstep use
uniform vec3 lightDir;
uniform vec3 lightColor;
//uniform vec3 meshColor;
uniform vec3 ambientColor;

uniform float nearPlane;
uniform float midPlane;
uniform bool needsSmoothstep;

uniform sampler2D tex;

void main()
{
    // camDistance is only needed if smoothstep is needed, and smoothTest will be used as an alpha value, 
    // so it only goes down below 1 if smoothstep is needed
    float camDistance = length(cameraPos);
    float smoothTest = 1;
    if (needsSmoothstep)
        smoothTest = 1 - smoothstep(nearPlane, midPlane, camDistance);

    vec3 normal = normalize(fragNormal);

    float nDotL = max(0, dot(normal, lightDir));
    vec3 irradiance = ambientColor + lightColor * nDotL; // how much light the surface effictively receives

    vec4 meshColor = pow(texture(tex, fragUV), vec4( 1.0));

    vec3 linearColor = meshColor.rgb * irradiance;

    outColor = vec4(pow(linearColor, vec3(1.0 / 2.2)), smoothTest);
}
