#version 460 core

layout(location = 0) out highp vec4 color;

uniform vec3 u_Color = vec3(0.0, 1.0, 1.0);

void main()
{
	color = vec4(u_Color, 0.2);
}