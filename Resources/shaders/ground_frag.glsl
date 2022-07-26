#version 460 core

layout(location = 0) out highp vec4 color;

uniform float u_Scale;
uniform uint u_Seed = 0;
uniform vec3 u_StdBrown = vec3(145/255, 55/255, 3/255);
uniform int u_Pixelation = 150; // Somewhere between 50 and 200 seems to be an appropriate value

in vec2 v_texCoord;

vec2 random2( vec2 p ) {
    return fract(sin(vec2(dot(p,vec2(127.1 + float(u_Seed),311.7)),dot(p,vec2(269.5,183.3))))*43758.5453);
}

vec2 voronoi_noise(int pixelation) {
    ivec2 int_texCoord = ivec2(int(v_texCoord.x * pixelation), int(v_texCoord.y * pixelation));
    vec2 st = floor(int_texCoord) / float(pixelation);

    // Scale
    st *= u_Scale;

    // Tile the space
    vec2 i_st = floor(st);
    vec2 f_st = fract(st);

    float m_dist = 1.;  // minimum distance
    vec2 m_point;

    for (int y= -1; y <= 1; y++) {
        for (int x= -1; x <= 1; x++) {
            // Neighbor place in the grid
            vec2 neighbor = vec2(float(x), float(y));

            // Random position from current + neighbor place in the grid
            vec2 point = random2(i_st + neighbor);

			// Vector between the pixel and the point
            vec2 diff = neighbor + point - f_st;

            // Distance to the point
            float dist = length(diff);
            
            if (dist < m_dist) {
                m_dist = dist;

                m_point = point;
            }
        }
    }
    
    return m_point;
}

float cellular_noise() {
    vec2 st = v_texCoord;

    // Scale
    st *= u_Scale;

    // Tile the space
    vec2 i_st = floor(st);
    vec2 f_st = fract(st);

    float m_dist = 1.;  // minimum distance

    for (int y= -1; y <= 1; y++) {
        for (int x= -1; x <= 1; x++) {
            // Neighbor place in the grid
            vec2 neighbor = vec2(float(x), float(y));

            // Random position from current + neighbor place in the grid
            vec2 point = random2(i_st + neighbor);

			// Vector between the pixel and the point
            vec2 diff = neighbor + point - f_st;

            // Distance to the point
            float dist = length(diff);

            // Keep the closer distance
            m_dist = min(m_dist, dist);
        }
    }
    
    return m_dist;
}

void main() {
    float ovalue = voronoi_noise(u_Pixelation).x;
    if (ovalue < 0.3) { ovalue = 0; }

    vec3 ccvalues = u_StdBrown * ovalue * max(v_texCoord.y - 0.3, 0.0);
    color = vec4(ccvalues.xyz, 1.0);
}