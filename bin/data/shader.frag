#version 410

uniform float distance2;
uniform float distance1;

in vec2 fragUV; 
in vec3 fragNormal;
in vec4 cameraPos;



//float m = smoothstep(10.0f, 0.1f, distance1);

out vec4 outColor;	



void main()
{

	float distanceTest = sqrt(pow(cameraPos[0],2) + pow(cameraPos[1], 2) + pow(cameraPos[2], 2));
	float camDistance = length(cameraPos);

	float smoothTest = 1 - smoothstep(0.1f, 10.0f, camDistance);
	vec3 normal = normalize(fragNormal);

	outColor = vec4(normal, smoothTest);	
}

