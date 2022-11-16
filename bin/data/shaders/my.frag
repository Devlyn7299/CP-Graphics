#version 410

uniform vec3 lightDir;
uniform vec3 lightColor;
uniform vec3 meshColor;
uniform vec3 ambientColor;

uniform sampler2D diffuseTex;
uniform sampler2D normalTex;

in vec2 fragUV;
in mat3 TBN;

out vec4 outColor;

void main()
{
	// Extracting color from texture
	// Gamma correction: perceptualy linear to physically linear (decoding gamma)
	vec3 diffuseColor = pow(texture(diffuseTex, fragUV).rgb, vec3(2.2));

	// Normal mapping
	vec3 tsNormal = texture(normalTex, fragUV).xyz * 2 - 1;
	vec3 wsNormal = normalize(TBN * tsNormal);

	vec3 normal; //= normalize(fragNormal);

	float nDotL = max(0.0, dot(wsNormal, lightDir));

	// how much light is effectively received by the surface
	vec3 irradiance = ambientColor + lightColor * nDotL;

	// Gamma correction: physically-linera to perceptually-linear (encoding gamma)
	outColor = vec4(pow(diffuseColor * irradiance, vec3(1.0 / 2.2)), 1.0);
	// outColor = vec4(wsNormal * 0.5 + 0.5, 1);

}
