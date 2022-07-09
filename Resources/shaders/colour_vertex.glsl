#version 460 core

layout(location = 0) in vec4 position;

uniform mat4 u_projMat;
uniform vec2 u_Offset;
uniform float u_ZCoord = 0.0;

void main() {
	gl_Position = u_projMat * vec4(position.xy + u_Offset, position.z, position.w);
}