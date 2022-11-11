#version 460 core

// TODO: needs like 3 different texture coordinates

layout(location = 0) in vec2 position;
layout(location = 1) in vec2 texCoord;
layout(location = 2) in vec2 healthbarTexCoord;
layout(location = 3) in vec2 maskTexCoord;

uniform mat4 u_ProjMat;

out vec2 v_texCoord;
out vec2 v_healthbarTexCoord;
out vec2 v_maskTexCoord;

void main() {
	gl_Position = u_ProjMat * vec4(position.xy, 0.0, 1.0);
	v_texCoord = texCoord;
	v_healthbarTexCoord = healthbarTexCoord;
	v_maskTexCoord = maskTexCoord;
}