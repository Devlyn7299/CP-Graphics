#version 410

uniform sampler2D spark;
uniform vec4 addedColor;
uniform float brightness;

in vec2 fragUV;
out vec4 outCol;

void main() {
	outCol = brightness * (texture(spark, fragUV) + vec4(0,0,0,0) + addedColor);
}