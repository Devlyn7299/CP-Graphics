#version 410

//in vec2 fragUV;
in vec3 fragNormal;
in vec3 worldPosition;

out vec4 outColor;

uniform vec3 meshColor;
uniform vec3 ambientColor;

// Uniform variables for directional lights
uniform vec3 dirLightDir;
uniform vec3 dirLightColor;

// Uniform variables for point lights
uniform vec3 pointLightPos;
uniform vec3 pointLightColor;

// Uniform variables for spot lights
uniform vec3 spotLightColor;
uniform vec3 spotLightConeDir;
uniform vec3 spotLightPos;
uniform float spotLightCutoff;

void main()
{
    vec3 normal = normalize(fragNormal);

    // Calculate directional lighting
    float dirNDotL = max(0, dot(normal, dirLightDir));
    vec3 directionalLight = dirLightColor * dirNDotL;

    // Calculate point lighting
    vec3 pointLightDir = normalize(pointLightPos - worldPosition);
    float pointNDotL = max(0, dot(normal, pointLightDir));
    vec3 pointLight = pointLightColor * pointNDotL;

    // Calculate spot lighting
    vec3 toSpotLight = spotLightPos - worldPosition;
    vec3 spotLightDir = normalize(toSpotLight);
    float cosAngle = dot(spotLightDir, -spotLightConeDir);
    float spotNDotL = max(0, dot(normal, spotLightDir));
    vec3 spotLight;
    if (cosAngle > spotLightCutoff) 
    {
        spotLight = spotLightColor * spotNDotL;
    }

    // how much light the surface effectively receives
    vec3 irradiance;
    if (cosAngle > spotLightCutoff)
        irradiance = ambientColor + directionalLight + pointLight + spotLight;
    else 
        irradiance = ambientColor + directionalLight + pointLight;

    // how much light the surface reflects (physically)
    vec3 linearColor = meshColor * irradiance;

    // gamma encoding
    outColor = vec4(pow(linearColor, vec3(1.0/2.2)), 1.0);
}
