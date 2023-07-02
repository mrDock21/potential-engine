#version 330 core

in vec2 texCoord;
in vec3 normal;
in vec4 fragPos;
out vec4 FragColor;

// textures
uniform sampler2D diffuseTexture;
uniform sampler2D specularTexture;
uniform sampler2D emissionTexture;

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
struct Material {
	float shininess;
};

// light and main material
uniform Light light;
uniform Material material;

// uniform block for global lights
//	binding Index = 1 (see Scene.hpp)
layout(std140) uniform GlobalLights{
	PointLight lights[10];
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

	color = computeDirectionalLight(normal, viewDir); 

	for (int i = 0; i < numLights; i++) {
		color += computePointLight(normal, lights[i], viewDir, fragPos.xyz);
	}

	FragColor = vec4(color, 1.0);
}

vec3 computeDirectionalLight(vec3 normal, vec3 viewDir) {
	vec3 lightDir = normalize(sun.direction);
	// diffuse
	float diffuseFactor = max(dot(normal, -lightDir), 0);

	// specular
	vec3 reflectDir = reflect(-lightDir, normal);
	float specularFactor =
		pow(clamp(dot(reflectDir, viewDir), 0.0, 1.0), material.shininess);
	float specularFlag = texture(specularTexture, texCoord).r;

	// emission filter
	vec3 emissionColor = texture(emissionTexture, texCoord).rgb;
	float emissionFlag = step(0.1, emissionColor.r);

	// ambient
	vec3 texColor = texture(diffuseTexture, texCoord).rgb,
		color = sun.ambient * texColor;
	// diffuse
	color += diffuseFactor * texColor * sun.color;
	// specular
	color += specularFactor * sun.color * specularFlag;
	// emission
	color += (1.0 - step(0.1, specularFlag)) * emissionFlag * emissionColor;

	return color;
}

vec3 computePointLight(vec3 normal, PointLight pLight, vec3 viewDir, vec3 fragPos) {
	// point light calculations
	vec3 lightDir = normalize(light.position - fragPos);
	float k1 = 0.09f, kq = 0.032f;
	float dist = distance(pLight.position, fragPos),
		att = 1.0f / (1.0f + k1 * dist + kq * (dist * dist));

	// diffuse
	float diffuseFactor = max(dot(normal, lightDir), 0);

	// specular
	vec3 reflectDir = reflect(lightDir, normal);
	float specularFactor =
		pow(clamp(dot(reflectDir, viewDir), 0.0, 1.0), material.shininess);
	float specularFlag = texture(specularTexture, texCoord).r;

	// emission filter
	vec3 emissionColor = texture(emissionTexture, texCoord).rgb;
	float emissionFlag = step(0.1, emissionColor.r);

	// ambient
	vec3 texColor = texture(diffuseTexture, texCoord).rgb,
		color = pLight.color * 0.1 * texColor;
	// diffuse
	color += diffuseFactor * texColor * pLight.color * att;
	// specular
	color += specularFactor * pLight.color * specularFlag * att;
	// emission
	color += (1.0 - step(0.1, specularFlag)) * emissionFlag * emissionColor;

	return color;
}

/*void mainOld() {
	// diffuse compute
	// light position should be in VIEW SPACE
	vec3 lightDir = normalize(light.position - fragPos.xyz);

	float k1 = 0.09f, kq = 0.032f;

	float dist = distance(light.position, fragPos.xyz),
		att = 1.0f / (1.0f + k1 * dist + kq * (dist * dist));
	float intensity = clamp(dot(lightDir, normal), 0.0, 1.0);

	// specular compute
	vec3 viewDir = normalize(vec3(0) - fragPos.xyz); // from frag to camera
	float specular = pow(clamp(dot(reflect(-lightDir, normal), viewDir), 0.0, 1.0), material.shininess);
	float specularFlag = texture(specularTexture, texCoord).r;

	// emission filter
	vec3 emissionColor = texture(emissionTexture, texCoord).rgb;
	float emissionFlag = step(0.1, emissionColor.r);

	// check spotlight flag (anything outside the angle is dark)
	float spotlightFlag = 1.0f - step(
		dot(-lightDir, light.forward), // get angle between vectors
		light.maxAngle				   // compare if result angle exceeds the limit
	);

	// color combination
	vec3 color, texColor = texture(diffuseTexture, texCoord).rgb;
	// ambient
	color = light.ambient.rgb * texColor;
	// diffuse
	color += intensity * texColor * light.color.rgb * att;
	// emission
	color += (1.0 - step(0.1, specularFlag)) * emissionFlag * emissionColor;
	// specular
	color += light.specular.rgb * specularFlag * light.color.rgb *
		specular * att;

	FragColor = vec4(color, 1.0);
}*/

