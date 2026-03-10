// PBR Shader with Shadow Mapping

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
uniform mat4 u_LightSpaceMatrix;

out vec4 v_Color;
out vec2 v_TexCoord;
out vec3 v_Normal;
out vec3 v_WorldPos;
out vec4 v_LightSpacePos;

void main() {
	vec4 worldPos = u_Model * vec4(a_Position, 1.0);
	v_WorldPos = worldPos.xyz;
	v_Color = a_Color;
	v_TexCoord = a_TexCoord;
	v_Normal = normalize(mat3(transpose(inverse(u_Model))) * a_Normal);
	v_LightSpacePos = u_LightSpaceMatrix * worldPos;
	gl_Position = u_ProjectionView * worldPos;
}

#type fragment
#version 420 core

layout(location = 0) out vec4 color;

#define MAX_POINT_LIGHTS 4
const float PI = 3.14159265359;

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
in vec4 v_LightSpacePos;

uniform sampler2D u_Texture;
uniform sampler2DShadow u_ShadowMap;
uniform vec4 u_Tint = vec4(1.0);
uniform float u_Metallic = 0.0;
uniform float u_Roughness = 0.5;

// ------------- PBR Functions -----------------

// Normal Distribution Function: GGX/Trowbridge-Reitz
float DistributionGGX(vec3 N, vec3 H, float roughness) {
	float a = roughness * roughness; // Disney remap: alpha = roughness^2
	float a2 = a * a;
	float NdotH = max(dot(N, H), 0.0);
	float NdotH2 = NdotH * NdotH;

	float denom = (NdotH2 * (a2 - 1.0) + 1.0);
	denom = PI * denom * denom;

	return a2 / max(denom, 0.0000001);
}

// Geometry Function: Schlick-GGX (single direction)
float GeometrySchlickGGX(float NdotV, float roughness) {
	float r = roughness + 1.0;
	float k = (r * r) / 8.0; // direct lighting remapping
	return NdotV / (NdotV * (1.0 - k) + k);
}

// Smith's method: combines view + light masking (correlated)
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness) {
	float NdotV = max(dot(N, V), 0.0);
	float NdotL = max(dot(N, L), 0.0);
	float ggx1 = GeometrySchlickGGX(NdotV, roughness);
	float ggx2 = GeometrySchlickGGX(NdotL, roughness);
	return ggx1 * ggx2;
}

// Fresnel: Schlick approximation
vec3 FresnelSchlick(float cosTheta, vec3 F0) {
	return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}

// --------------- Radiance Calculation --------------------
vec3 CalcPBRLight(vec3 N, vec3 V, vec3 L, vec3 radiance, vec3 albedo, float metallic, float roughness, vec3 F0) {
	vec3 H = normalize(V + L);

	// Cook-Torrance BRDF
	float D = DistributionGGX(N, H, roughness);
	float G = GeometrySmith(N, V, L, roughness);
	vec3  F = FresnelSchlick(max(dot(H, V), 0.0), F0);

	// Specular
	vec3 numerator = D * G * F;
	float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.0001;
	vec3 specular = numerator / denominator;

	// Energy conservation: diffuse + specular <= 1
	vec3 kS = F; // reflected portion
	vec3 kD = vec3(1.0) - kS; // refracted/diffuse portion
	kD *= 1.0 - metallic; // metals have no diffuse

	float NdotL = max(dot(N, L), 0.0);
	return (kD * albedo / PI + specular) * radiance * NdotL;
}

// --------------- Shadow Calculation ---------------------
float CalcShadow(vec4 lightSpacePos, vec3 N, vec3 L) {
	// Perspective divide (identity for ortho, but good practice)
	vec3 projCoords = lightSpacePos.xyz / lightSpacePos.w;
	projCoords = projCoords * 0.5 + 0.5; // [-1,1] -> [0,1]

	// Outside shadow frustum -> fully lit
	if (projCoords.z > 1.0) { return 0.0; }

	// Bias to fight shadow acne - scales with angle between normal and light
	float bias = max(0.005 * (1.0 - dot(N, L)), 0.001);
	float biasedDepth = projCoords.z - bias;

	// PCF: 3x3 kernel for soft edges
	float shadow = 0.0;
	vec2 texelSize = 1.0 / textureSize(u_ShadowMap, 0);
	for (int x = -1; x <= 1; ++x)
	for (int y = -1; y <= 1; ++y) {
		vec2 offset = vec2(x, y) * texelSize;
		// sampler2DShadow: texture() returns 0.0 (in shadow) or 1.0 (lit)
		shadow += texture(u_ShadowMap, vec3(projCoords.xy + offset, biasedDepth));
	}
	shadow /= 9.0;

	// shadow = 1.0 means fullt lit, 0.0 means fully shadowed
	// we want to return the "shadow factor" (0 = lit, 1 = shadow)
	return 1.0 - shadow;
}

// ----------------- Main ----------------------

void main() {
	vec4 texColor = texture(u_Texture, v_TexCoord) * v_Color * u_Tint;
	vec3 albedo = texColor.rgb;

	vec3 N = normalize(v_Normal);
	vec3 V = normalize(u_CameraPosition.xyz - v_WorldPos);

	// Metallic surfaces use albedo as F0, dielectrics use 0.04
	vec3 F0 = mix(vec3(0.04), albedo, u_Metallic);

	// --- Ambient (very simple IBL stand-in, unaffected by shadows) ---
	vec3 ambient = u_Ambient.rgb * u_Ambient.a * albedo;

	// --- Directional Light (with shadows) ---
	vec3 dirL = normalize(-u_LightDirection.xyz);
	vec3 dirRadiance = u_LightColor.rgb * u_LightColor.a;
	vec3 dirContribution = CalcPBRLight(N, V, dirL, dirRadiance, albedo, u_Metallic, u_Roughness, F0);

	float shadow = CalcShadow(v_LightSpacePos, N, dirL);
	vec3 Lo = dirContribution * (1.0 - shadow);

	// --- Point Lights (no shadows for now) ---
	int count = int(u_PointLightCount.x);
	for (int i = 0; i < MAX_POINT_LIGHTS; ++i) {
		if (i >= count) { break; }

		vec3  lightPos 	 = u_PointLights[i].Position.xyz;
		float radius 	 = u_PointLights[i].Position.w;
		vec3  lightColor = u_PointLights[i].Color.rgb;
		float intensity  = u_PointLights[i].Color.a;

		vec3 toLight = lightPos - v_WorldPos;
		float dist = length(toLight);
		vec3 L = toLight / dist;

		// Smooth radius-based attenuation
		float attenuation = clamp(1.0 - (dist * dist) / (radius * radius), 0.0, 1.0);
		attenuation *= attenuation; // smoother falloff

		vec3 radiance = lightColor * intensity * attenuation;
		Lo += CalcPBRLight(N, V, L, radiance, albedo, u_Metallic, u_Roughness, F0);
	}

	vec3 result = ambient + Lo;

	// HDR tone mapping (Reinhard) + gamma correction
	result = result / (result + vec3(1.0)); // Reinhard
	result = pow(result, vec3(1.0 / 2.2)); // Gamma

	color = vec4(result, texColor.a);
}