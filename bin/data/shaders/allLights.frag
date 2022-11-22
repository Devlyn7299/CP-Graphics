#version 410

uniform vec3 lightDir;
uniform vec3 lightColor;
uniform vec3 ambientColor;

uniform vec3 cameraPosition;

uniform vec3 dirLightDir;
uniform vec3 dirLightColor;

uniform vec3 pointLightPosition;
uniform vec3 pointLightColor;

uniform vec3 spotLightColor;
uniform vec3 spotLightConeDir;
uniform vec3 spotLightPos;
uniform float spotLightCutoff;

uniform sampler2D diffuseTex;
uniform sampler2D normalTex;
uniform sampler2D specularTex;

uniform float shininess;

uniform samplerCube envMap;

in vec2 fragUV;
in vec3 fragWorldPos;
in mat3 TBN;

out vec4 outColor;

void main()
{
    // Normal mapping
    vec3 tsNormal = texture(normalTex, fragUV).xyz * 2 - 1;
    vec3 wsNormal = normalize(TBN * tsNormal);

    float dirNDotL = max(0.0, dot(wsNormal, dirLightDir)); // assuming lightDir pre-normalized


    // Setting up variables for point light and spot light
    // Point light
    vec3 pointToLight = pointLightPosition - fragWorldPos;
    vec3 pointLightDir = normalize(pointToLight);
    float pointFalloff = 1 / dot(pointToLight, pointToLight);
    float pointNDotL = max(0.0, dot(wsNormal, pointLightDir));

    // Spot light
    vec3 toLight = spotLightPos - fragWorldPos;
    vec3 spotLightDir = normalize(toLight);
    float falloff;
    float cosAngle = dot(spotLightConeDir, -spotLightDir);
    falloff = max(0, sign(cosAngle - spotLightCutoff)) / dot(toLight, toLight);
    float spotNDotL = max(0.0, dot(wsNormal, spotLightDir));


    // Extracting metallicity from texture
    float metallicity = texture(specularTex, fragUV).r;
    //float metallicitySmoothstep = smoothstep(0, 1, texture(specularTex, fragUV).b);
    float roughness = 0.5;

    vec3 baseColor = pow(texture(diffuseTex, fragUV).rgb, vec3(2.2));
    // Extacting color from texture
    // Gamma correcion: perceptually linear to physically linear (decoding gamma)
    //vec3 diffuseColor = pow(texture(diffuseTex, fragUV).rgb, vec3(2.2));
    vec3 diffuseColor = baseColor * (1 - metallicity);
    
    // Extracting specular color from texture w/ gamma correction, and adding in directional, spot, and point lights
    vec3 specularColor =mix(vec3(0.04), baseColor  + dirLightColor * dirNDotL + spotLightColor * spotNDotL * falloff + pointLightColor * pointNDotL * pointFalloff, vec3(metallicity));


    // DIFFUSE CALCULATIONS

    // irradiance - decoding gamma
    vec3 envIrradiance = pow(textureLod(envMap, wsNormal, 99).rgb, vec3(2.2));

    vec3 irradiance = ambientColor + envIrradiance + dirLightColor * dirNDotL + pointLightColor * pointNDotL * pointFalloff + spotLightColor * falloff * spotNDotL;

    // Calculate view vector (direction from surface to camera)
    vec3 view = normalize(cameraPosition - fragWorldPos);
    vec3 envLightDir = reflect(-view, wsNormal);

    vec3 ambientDiffuse = diffuseColor * irradiance;


    // SPECULAR CALCULATIONS

    // environment map reflection - decoding gamma
    vec3 envReflection = pow(texture(envMap, envLightDir).rgb, vec3(2.2));

    // Fresnel Effect calculation
    float envNDotL = max(0.0, dot(wsNormal, envLightDir));

    vec3 fresnel = mix(specularColor, vec3(1), pow(1 - envNDotL, 5));

    // Specular reflection calculation
    vec3 specularReflection = fresnel * envReflection;


    // Specular highlight calculation
    vec3 halfway = normalize(spotLightDir * dirLightDir * pointLightDir + view);
    float nDotH = max(0.0, dot(wsNormal, halfway));
    
    // Calculate GGX
    float GGX = (roughness * roughness)/(pow(mix(1, roughness* roughness, nDotH * nDotH), 2));

    // Calculate Smith 
    float nDotV = max(0.0, dot(wsNormal, view));
    float smith = 0.5 / max(0.01, (nDotV * sqrt(mix((spotNDotL * dirNDotL * pointNDotL) * (spotNDotL * dirNDotL * pointNDotL), 1, roughness * roughness)) 
        + (spotNDotL * dirNDotL * pointNDotL) * sqrt(mix(nDotV * nDotV, 1, roughness * roughness))));

    // Fresnel Effect calculation for highlights
    float hDotL = max(0.0, dot(halfway, dirLightDir * spotLightDir * pointLightDir));
    vec3 fresnelHighlight = mix(specularColor, vec3(1), pow(1 - hDotL, 5));

    // Calculate Cook-Torrance
    vec3 cookTorrance = (GGX * smith * fresnelHighlight);

    vec3 specularHighlight = cookTorrance * dirLightColor * dirNDotL * spotLightColor * spotNDotL * falloff * pointLightColor * pointNDotL * pointFalloff;


    // Add ambient, diffuse, and specular reflections and highlights
    vec3 totalColor = ambientDiffuse + specularReflection + specularHighlight;

    // Gamma correction: physically-linear to perceptually-linear (encoding gamma)
    outColor = vec4(pow(totalColor, vec3(1.0 / 2.2)), 1.0);
}
