#version 460 core
layout (location = 0) in vec4 vertex; // <vec2 pos, vec2 tex>
out vec2 v_TexCoords;

uniform mat4 u_projMat;
uniform vec2 u_Offset;
uniform float u_ZCoord = 0.0;

void main()
{
    gl_Position = u_projMat * vec4(vertex.xy, 0.0, 1.0);
    v_TexCoords = vertex.zw;
} 