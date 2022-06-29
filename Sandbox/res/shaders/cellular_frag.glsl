#version 460 core

layout(location = 0) out highp vec4 color;

uniform vec2 u_Resolution;
uniform float u_Scale;
uniform uint u_Seed;

vec2 random2( vec2 p ) {
    return fract(sin(vec2(dot(p,vec2(127.1 + float(u_Seed),311.7)),dot(p,vec2(269.5,183.3))))*43758.5453);
}

vec2 voronoi_noise() {
    vec2 st = gl_FragCoord.xy/u_Resolution.xy;
    st.x *= u_Resolution.x/u_Resolution.y;

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
    vec2 st = gl_FragCoord.xy/u_Resolution.xy;
    st.x *= u_Resolution.x/u_Resolution.y;

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
    vec2 ovalue = voronoi_noise();
    color = vec4(ovalue.xxx, 1.0);
}