#version 460 core

layout(location = 0) in vec4 position;

uniform mat4 u_projMat;
uniform mat4 u_viewMat;

void main() {
	gl_Position = u_viewMat * position;
}