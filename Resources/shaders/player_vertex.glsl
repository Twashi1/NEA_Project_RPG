#version 460 core

layout(location = 0) in vec4 position;

uniform mat4 u_projMat;
uniform vec2 u_OffsetCam;

void main() {
	gl_Position = u_projMat * vec4(position.xy + u_OffsetCam, position.z, position.w);
}