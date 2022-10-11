#version 410

uniform float distance2;
uniform float distance1;

in vec2 fragUV; 
in vec3 fragNormal;
in vec4 cameraPos;

out vec4 outColor;	

void main()
{
	float camDistance = length(cameraPos);
	float smoothTest = 1 - smoothstep(0.1f, 10.0f, camDistance);
	vec3 normal = normalize(fragNormal);
	outColor = vec4(normal, smoothTest);	
}
