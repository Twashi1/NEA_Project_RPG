#version 460 core

layout(location = 0) out highp vec4 color;

in vec2 v_texCoord;
//in int v_ItemCount;

uniform sampler2D u_Texture;
uniform float u_Scale = 1.0f;

void main()
{
	vec4 texColor = texture(u_Texture, v_texCoord / u_Scale);

//	if (v_ItemCount == 2) {
//		texColor += texture(u_Texture, (v_texCoord + vec2(0.2f, 0.3f)) / u_Scale); // Repeat texture, but move it up a bit and to the right
//	}
//
//	if (v_ItemCount >= 3) {
//		texColor += texture(u_Texture, (v_texCoord + vec2(-0.2f, 0.3f)) / u_Scale); // Repeat texture, but move it up a bit and to the left
//	}

	color = texColor;
}