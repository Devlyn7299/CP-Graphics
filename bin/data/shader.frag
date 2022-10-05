#version 410

in vec2 fragUV; // from vertex shader via rasterizer
in vec3 fragNormal;
out vec4 outColor;	// to frame buffer (usually screen)


void main()
{
	vec3 normal = normalize(fragNormal);
	outColor = vec4(fragNormal * 0.5 + vec3(1.0) * 0.5, 1.0);	
}