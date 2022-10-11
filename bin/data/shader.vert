#version 410

layout (location=0) in vec3 position;	
layout (location = 2) in vec3 normal;
layout (location=3) in vec2 uv;		

uniform mat4 mvp;
uniform mat4 modelView;

out vec3 fragNormal;
out vec2 fragUV;	
out vec4 cameraPos;

void main()
{
	gl_Position = mvp * vec4(position, 1.0); 
	fragNormal = normal;
	fragUV = vec2(uv.x, 1.0-uv.y); 
	cameraPos = modelView * vec4(position, 1.0);
}
