#version 460 core
layout (location = 0) in vec4 vertex; // <vec2 pos, vec2 tex>
out vec2 v_TexCoords;

uniform mat4 u_projMat;
uniform mat4 u_viewMat;

void main()
{
    gl_Position = u_viewMat * vec4(vertex.xy, 0.0, 1.0);
    v_TexCoords = vertex.zw;
} 