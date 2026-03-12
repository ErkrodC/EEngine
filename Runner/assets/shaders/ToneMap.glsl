// ACES Filmic Tone Mapping + Gamma Correction

#type vertex
#version 420 core

// Fullscreen triangle trick - no vertex buffer needed
// 3 vertices: (-1,-1), (3,-1), (-1,3) covers the entire screen
out vec2 v_TexCoord;

void main() {
    // Generate fullscreen triangle from the vertex ID
    v_TexCoord = vec2((gl_VertexID << 1) & 2, gl_VertexID & 2);
    gl_Position = vec4(v_TexCoord * 2.0 - 1.0, 0.0, 1.0);
}

#type fragment
#version 420 core

layout(location = 0) out vec4 FragColor;

in vec2 v_TexCoord;

uniform sampler2D u_HDRBuffer;
uniform float u_Exposure = 1.0;

// ACES filmic tone mapping (Stephen Hill's fit)
// Much better contrast and color preservation than Reinhard
vec3 ACESFilmic(vec3 x) {
    float a = 2.51;
    float b = 0.03;
    float c = 2.43;
    float d = 0.59;
    float e = 0.14;
    return clamp((x * (a * x + b)) / (x * (c * x + d) + e), 0.0, 1.0);
}

void main() {
    vec3 hdrColor = texture(u_HDRBuffer, v_TexCoord).rgb;

    // Exposure adjustment (lets you control overall brightness)
    vec3 mapped = ACESFilmic(hdrColor * u_Exposure);

    // Gamma correction
    mapped = pow(mapped, vec3(1.0 / 2.2));

    FragColor = vec4(mapped, 1.0);
}