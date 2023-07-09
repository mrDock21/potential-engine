#version 330 core

in vec2 texCoord;
in vec3 normal;
in vec4 fragPos;
in mat4 viewMatrix;
out vec4 FragColor;

// textures
uniform sampler2D diffuseTexture;

// structs
struct PointLight {
	// position should be in view space
	vec3 position;
	float k1; 
	vec3 color;
	float kq;
};
struct SunLight {
	vec4 direction;
	vec4 color;
	vec4 ambient;
};

// uniform block for global lights
#define MAX_LIGHTS 10
//	binding Index = 1 (see Scene.hpp)
layout(std140) uniform GlobalLights {
	PointLight lights[MAX_LIGHTS];
	SunLight sun;
	float numLights;
};

vec3 computeDirectionalLight(vec3 normal, vec3 viewDir);
vec3 computePointLight(vec3 normal, PointLight pLight, vec3 viewDir, vec3 fragPos);

/**
* This shader is meant to be used by the 3D objects, NOT the light itself
*/

void main() {

	// color combination
	vec3 color, texColor = texture(diffuseTexture, texCoord).rgb;
	// from frag to camera)
	vec3 viewDir = normalize(vec3(0) - fragPos.xyz);
	
	vec3 std_normal = normalize(normal);
	
	color = computeDirectionalLight(std_normal, viewDir);

	for (int i = 0; i < numLights; i++) {
		color += computePointLight(std_normal, lights[i], viewDir, fragPos.xyz);
	}

	FragColor = vec4(color, 1.0);
}

vec3 computeDirectionalLight(vec3 normal, vec3 viewDir) {
	vec3 lightDir = normalize( ( viewMatrix * vec4(sun.direction.xyz, 0.0) ).xyz );
	// diffuse
	float diffuseFactor = max(dot(normal, -lightDir), 0);

	// specular
	vec3 reflectDir = reflect(lightDir, normal);
	float specularFactor =
		pow(clamp(dot(reflectDir, viewDir), 0.0, 1.0), 128.0);

	// ambient
	vec3 texColor = texture(diffuseTexture, texCoord).rgb,
		color = sun.ambient.xyz * texColor;
	// diffuse
	color += diffuseFactor * texColor * sun.color.xyz;
	// specular
	color += specularFactor * sun.color.xyz;

	return color;
}

vec3 computePointLight(vec3 normal, PointLight pLight, vec3 viewDir, vec3 fragPos) {
	// point light calculations
	vec3 lPos = (viewMatrix * vec4(pLight.position, 1.0)).xyz;
	vec3 frag2LightDir = normalize(lPos - fragPos);
	float k1 = 0.09f, kq = 0.032f;
	float dist = distance(lPos, fragPos),
		att = 1.0f / (1.0f + k1 * dist + kq * (dist * dist));

	// diffuse
	float diffuseFactor = max(dot(normal, frag2LightDir), 0);

	// specular
	vec3 reflectDir = reflect(frag2LightDir, normal);
	float shininess = 128.0, // this one can be set as uniform...
		  specularFactor = pow(clamp(dot(reflectDir, viewDir), 0.0, 1.0), shininess);

	// ambient
	vec3 texColor = texture(diffuseTexture, texCoord).rgb,
		color = pLight.color * 0.1 * texColor;
	// diffuse
	color += diffuseFactor * texColor * pLight.color * att;
	// specular
	color += specularFactor * pLight.color * att;

	return color;
}
