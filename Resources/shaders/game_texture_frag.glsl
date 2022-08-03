#version 460 core

layout(location = 0) out highp vec4 color;

in vec2 v_texCoord;

uniform float u_Scale = 1.0f;

uniform sampler2D u_Texture;

void main()
{
	vec4 texColor = texture(u_Texture, v_texCoord / u_Scale);
	color = texColor;
}