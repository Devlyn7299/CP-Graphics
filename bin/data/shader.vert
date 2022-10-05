#version 410

layout (location=0) in vec3 position;	// from vertex buffer
layout (location = 2) in vec3 normal;
layout (location=3) in vec2 uv;		// from vertex buffer

out vec3 fragNormal;
out vec2 fragUV;	// to fragment shader via rasterizer

uniform mat4 mvp;

void main()
{
	gl_Position = mvp * vec4(position, 1.0); // to rasterizer
	fragNormal = normal;
	fragUV = vec2(uv.x, 1.0-uv.y); // flip texture // "pass through" from vertex buffer to rasterizer
}

