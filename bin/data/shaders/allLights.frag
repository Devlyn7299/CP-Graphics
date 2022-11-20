#version 410

uniform vec3 lightDir;
uniform vec3 lightColor;
uniform vec3 ambientColor;

uniform vec3 cameraPosition;

uniform vec3 dirLightDir;
uniform vec3 dirLightColor;

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

    








    // how much light is effectively received by the surface



    // Gamma correction: physically-linear to perceptually-linear (encoding gamma)




    // Extracting metallicity from texture
    float metallicity = texture(specularTex, fragUV).r;
    float metallicitySmoothstep = smoothstep(0, 1, texture(specularTex, fragUV).b);

    // Extacting color from texture
    // Gamma correcion: perceptually linear to physically linear (decoding gamma)
    //vec3 diffuseColor = pow(texture(diffuseTex, fragUV).rgb, vec3(2.2));
    vec3 diffuseColor = pow(texture(diffuseTex, fragUV).rgb * (1 - metallicity), vec3(2.2));
    
    // Extracting specular color from texture w/ gamma correction
    //vec3 specularColor = pow(texture(specularTex, fragUV).rgb, vec3(2.2));
    vec3 specularColor = pow(mix(vec3(0.04), texture(diffuseTex, fragUV).rgb, vec3(metallicity)), vec3(2.2));

    // Normal mapping
    vec3 tsNormal = texture(normalTex, fragUV).xyz * 2 - 1;
    vec3 wsNormal = normalize(TBN * tsNormal);

    float dirnDotL = max(0.0, dot(wsNormal, lightDir)); // assuming lightDir pre-normalized

    // DIFFUSE CALCULATIONS

    // Calculate spot lighting
    vec3 toSpotLight = spotLightPos - fragWorldPos;
    vec3 spotLightDir = normalize(toSpotLight);
    float cosAngle = dot(spotLightDir, -spotLightConeDir);
    float spotNDotL = max(0, dot(wsNormal, spotLightDir));
    vec3 spotLight;
    if (cosAngle > spotLightCutoff) 
    {
        spotLight = spotLightColor * spotNDotL;
    }

    // irradiance - decoding gamma
    vec3 envIrradiance = pow(textureLod(envMap, wsNormal, 99).rgb, vec3(2.2));

    // how much light is effectively received by the surface
    vec3 irradiance = ambientColor + envIrradiance + lightColor * dirnDotL;

    if (cosAngle > spotLightCutoff)
        irradiance = ambientColor + spotLight + envIrradiance + dirLightColor * dirnDotL;


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
    vec3 halfway = normalize(lightDir + view);
    float nDotH = max(0.0, dot(wsNormal, halfway));
    
    // Calculate GGX
    float GGX = pow(metallicitySmoothstep, 2)/(pow(mix(1, pow(metallicitySmoothstep, 2), pow(nDotH, 2)), 2));

    // Calculate Smith 
    float nDotV = max(0.0, dot(wsNormal, view));
    //float smith = 0.5 / mix(2*dirnDotL * nDotV, dirnDotL + nDotV, pow(metallicitySmoothstep, 2));
    float smith = 0.5 / (nDotV * sqrt(mix(dirnDotL*dirnDotL, 1, metallicitySmoothstep * metallicitySmoothstep)) + 
        dirnDotL * sqrt(mix(nDotV*nDotV, 1, metallicitySmoothstep * metallicitySmoothstep)));



    // Calculate highlight intensity (Blinn-Phong)
    float highlightIntensity = 0.25 * (1 + 0.5 * shininess) * pow(nDotH, shininess);

    // Fresnel Effect calculation
    float hDotL = max(0.0, dot(halfway, lightDir));
    vec3 fresnelHighlight = mix(specularColor, vec3(1), pow(1 - hDotL, 5));

    // Calculate Cook-Torrance
    vec3 cookTorrance = (3.14159 * GGX * smith * fresnelHighlight);

    vec3 specularHighlight = cookTorrance * dirLightColor * dirnDotL;

    // Add ambient, diffuse, and specular reflections and highlights
    vec3 totalColor = ambientDiffuse + specularReflection + specularHighlight;

    // Gamma correction: physically-linear to perceptually-linear (encoding gamma)
    outColor = vec4(pow(totalColor, vec3(1.0 / 2.2)), 1.0);
}
