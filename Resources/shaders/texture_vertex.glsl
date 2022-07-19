#version 460 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec2 texCoord;

uniform mat4 u_projMat;
uniform mat4 u_viewMat;

out vec2 v_texCoord;

void main() {
	gl_Position = u_projMat * u_viewMat * position;
	v_texCoord = texCoord;
}