#version 410

uniform sampler2D spark;

in vec2 fragUV;
out vec4 outCol;

void main() {
	outCol = texture(spark, fragUV) + vec4(0,0,1,0);
}