// Blinn-Phong Texture Shader

#type vertex
#version 420 core

layout(location = 0) in vec4 a_Color;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec3 a_Position;
layout(location = 3) in vec2 a_TexCoord;

layout(std140, binding = 0) uniform CameraData {
    mat4 u_ProjectionView;
    vec4 u_CameraPosition;
};

uniform mat4 u_Model;

out vec4 v_Color;
out vec2 v_TexCoord;
out vec3 v_Normal;
out vec3 v_WorldPos;

void main() {
    vec4 worldPos = u_Model * vec4(a_Position, 1.0);
    v_WorldPos = worldPos.xyz;
    v_Color = a_Color;
    v_TexCoord = a_TexCoord;
    v_Normal = normalize(mat3(transpose(inverse(u_Model))) * a_Normal);
    gl_Position = u_ProjectionView * worldPos;
}

#type fragment
#version 420 core

layout(location = 0) out vec4 color;

#define MAX_POINT_LIGHTS 4

struct PointLight {
    vec4 Position; // XYZ, W = radius
    vec4 Color; // RGB, A = intensity
};

layout(std140, binding = 0) uniform CameraData {
    mat4 u_ProjectionView;
    vec4 u_CameraPosition;
};

layout(std140, binding = 1) uniform LightData {
// Directional
    vec4 u_LightDirection;
    vec4 u_LightColor;
    vec4 u_Ambient;
// Point lights
    PointLight u_PointLights[MAX_POINT_LIGHTS];
    vec4 u_PointLightCount; // x = active count
};

in vec4 v_Color;
in vec2 v_TexCoord;
in vec3 v_Normal;
in vec3 v_WorldPos;

uniform sampler2D u_Texture;
uniform vec4 u_Tint = vec4(1.0);
uniform float u_Shininess = 32.0;
uniform float u_SpecularStrength = 0.5;

// Blinn-Phong for any light direction L
vec3 CalcBlinnPhong(vec3 N, vec3 L, vec3 V, vec3 lightColor, float lightIntensity) {
    float diff = max(dot(N, L), 0.0);
    vec3 diffuse = lightColor * lightIntensity * diff;

    vec3 H = normalize(L + V);
    float spec = pow(max(dot(N, H), 0.0), u_Shininess);
    vec3 specular = lightColor * lightIntensity * spec * u_SpecularStrength;

    return diffuse + specular;
}

void main() {
    vec4 texColor = texture(u_Texture, v_TexCoord) * v_Color * u_Tint;

    vec3 N = normalize(v_Normal);
    vec3 V = normalize(u_CameraPosition.xyz - v_WorldPos);

    // --- Ambient ---
    vec3 result = u_Ambient.rgb * u_Ambient.a;

    // --- Directional Light ---
    vec3 dirL = normalize(-u_LightDirection.xyz);
    result += CalcBlinnPhong(N, dirL, V, u_LightColor.rgb, u_LightColor.a);

    // --- Point Lights ---
    int count = int(u_PointLightCount.x);
    for (int i = 0; i < MAX_POINT_LIGHTS; ++i) {
        if (i >= count) { break; }

        vec3 lightPos = u_PointLights[i].Position.xyz;
        float radius = u_PointLights[i].Position.w;
        vec3 lightColor = u_PointLights[i].Color.rgb;
        float intensity = u_PointLights[i].Color.a;

        vec3 toLight = lightPos - v_WorldPos;
        float dist = length(toLight);
        vec3 L = toLight / dist;

        // Smooth attenuation: quadratic falloff, clamped to radius
        float attenuation = clamp(1.0 - (dist * dist) / (radius * radius), 0.0, 1.0);
        attenuation *= attenuation; // smoother falloff

        result += CalcBlinnPhong(N, L, V, lightColor, intensity) * attenuation;
    }

    color = vec4(result, 1.0) * texColor;
}