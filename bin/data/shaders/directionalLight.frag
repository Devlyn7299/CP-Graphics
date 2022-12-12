#version 410

uniform vec3 lightDir;
uniform vec3 lightColor;
uniform vec3 ambientColor;

uniform vec3 cameraPosition;

uniform sampler2D diffuseTex;
uniform sampler2D normalTex;
uniform sampler2D specularTex;

uniform float shininess;

uniform samplerCube envMap;

uniform int reflection;

in vec2 fragUV;
in vec3 fragWorldPos;
in mat3 TBN;

out vec4 outColor;

void main()
{


/*
    // if reflection is > zero and y-coordinate is less than zero.
    if (reflection * fragWorldPos.y < 0.0)
    {
        discard;
    }

    // Extacting color from texture
    // Gamma correcion: perceptually linear to physically linear (decoding gamma)
    vec3 diffuseColor = pow(texture(diffuseTex, fragUV).rgb, vec3(2.2));
    
    // Extacting specular color from texture w/ gamma correction
    vec3 specularColor = pow(texture(specularTex, fragUV).rgb, vec3(2.2));

    // Normal mapping
    vec3 tsNormal = texture(normalTex, fragUV).xyz * 2 - 1;
    vec3 wsNormal = normalize(TBN * tsNormal);

    float nDotL = max(0.0, dot(wsNormal, lightDir)); // assuming lightDir pre-normalized

    // DIFFUSE CALCULATIONS

    // irradiance - decoding gamma
    vec3 envIrradiance = pow(textureLod(envMap, wsNormal, 99).rgb, vec3(2.2));

    // how much light is effectively received by the surface
    vec3 irradiance = ambientColor + envIrradiance + lightColor * nDotL;

    // calculate view vector (direction from surface to camera)
    vec3 view = normalize(cameraPosition - fragWorldPos);
    vec3 envLightDir = reflect(-view, wsNormal); 
    
    vec3 ambientDiffuse = diffuseColor * irradiance;

    // SPECULAR CALCULATIONS

    // environment map reflection - decoding gamma
    vec3 envReflection = pow(texture(envMap, envLightDir).rgb, vec3(2.2));

    // Fresnel effect calculation
    float envNDotL = max(0.0, dot(wsNormal, envLightDir));
    vec3 fresnel = mix(specularColor, vec3(1), pow(1 - envNDotL, 5));

    // Specular reflection calculation
    vec3 specularReflection = fresnel * envReflection;

    // Specular highlight calculation
    vec3 halfway = normalize(lightDir + view);
    float nDotH = max(0.0, dot(wsNormal, halfway));

    // Calculate highlight intensity
    float highlightIntensity = 
        0.25 * (1 + 0.5 * shininess) * pow(nDotH, shininess);

    // Fresnel effect for highlights
    float hDotL = max(0.0, dot(halfway, lightDir));
    vec3 highlightFresnel = mix(specularColor, vec3(1), pow(1 - hDotL, 5));
    vec3 specularHighlight = highlightFresnel * highlightIntensity * nDotL;

    // Add ambient, diffuse, and specular reflections and highlights
    vec3 totalColor = ambientDiffuse + specularReflection + specularHighlight;

    // Gamma correction: physically-linear to perceptually-linear (encoding gamma)
    outColor = vec4(pow(totalColor, vec3(1.0 / 2.2)), 1.0);
*/

    vec3 diffuseColor = pow(texture(diffuseTex, fragUV).rgb, vec3(2.2));

    vec3 tsNormal = texture(normalTex, fragUV).xyz * 2 - 1;
    vec3 wsNormal = normalize(TBN * tsNormal);

    float nDotL = max(0.0, dot(wsNormal, lightDir));

    vec3 irradiance = ambientColor + lightColor * nDotL;

    outColor = vec4(pow(diffuseColor.rgb * irradiance, vec3(1.0 / 2.2)), 1.0);
}
