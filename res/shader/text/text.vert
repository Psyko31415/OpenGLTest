#version 330 core

layout(location=0) in vec4 data;

uniform mat4 projection;
out vec2 texcoord;

void main()
{
	gl_Position = projection * vec4(data.xy, 0.0, 1.0);
	texcoord = data.zw;
}
