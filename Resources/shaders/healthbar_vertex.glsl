#version 460 core

// TODO: needs like 3 different texture coordinates

layout(location = 0) in vec2 position;
layout(location = 1) in vec2 texCoord;

uniform mat4 u_ProjMat;

out vec2 v_texCoord;

void main() {
	gl_Position = u_ProjMat * vec4(position.xy, 0.0, 1.0);
	v_texCoord = texCoord;
}