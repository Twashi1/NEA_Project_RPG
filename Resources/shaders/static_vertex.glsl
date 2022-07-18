#version 460 core

layout(location = 0) in vec4 position;

uniform mat4 u_projMat;

void main() {
	gl_Position = u_projMat * position;
}