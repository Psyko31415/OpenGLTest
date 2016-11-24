#version 330 core

in vec2 texcoord;
out vec4 color;

uniform sampler2D tex;
uniform vec4 textColor;

void main() 
{
	vec4 sampled = vec4(1.0, 1.0, 1.0, texture(tex, texcoord).r);
	color = sampled * textColor;
}