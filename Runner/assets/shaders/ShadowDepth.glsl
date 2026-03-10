// Shadow Depth Shader - renders scene from light's POV, writes only depth

#type vertex
#version 420 core

layout(location = 2) in vec3 a_Position;

uniform mat4 u_LightSpaceMatrix;
uniform mat4 u_Model;

void main() {
    gl_Position = u_LightSpaceMatrix * u_Model * vec4(a_Position, 1.0);
}

#type fragment
#version 420 core

void main() {
    // Depth is written automatically - nothing to do here
}