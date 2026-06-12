#version 330 core

in vec2 texCoord;
in vec3 normal;
in vec4 fragPos;
in mat4 viewMatrix;
in vec4 lightFragPos;
in mat4 lightProjMatrix;

out vec4 FragColor;

// textures
uniform sampler2D diffuseTexture;

// shadows
uniform sampler2D depthTexture;

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
float computeShadow(vec3 _normal, vec3 lightDir);

/**
* This shader is meant to be used by the 3D objects, NOT the light itself
*/

void main() {

	// color combination
	vec4 fullColor = texture(diffuseTexture, texCoord);
	vec3 color, texColor = fullColor.rgb;
	// from frag to camera)
	vec3 viewDir = normalize(vec3(0) - fragPos.xyz);
	
	vec3 std_normal = normalize(normal);
	
	color = computeDirectionalLight(std_normal, viewDir);

	for (int i = 0; i < numLights; i++) {
		color += computePointLight(std_normal, lights[i], viewDir, fragPos.xyz);
	}

	FragColor = vec4(color, fullColor.a);
}

vec3 computeDirectionalLight(vec3 _normal, vec3 viewDir) {
	vec3 lightDir = normalize( ( viewMatrix * vec4(sun.direction.xyz, 0.0) ).xyz );
	// diffuse
	float diffuseFactor = max(dot(_normal, -lightDir), 0);

	// specular
	vec3 reflectDir = reflect(lightDir, _normal);
	float specularFactor =
		pow(clamp(dot(reflectDir, viewDir), 0.0, 1.0), 128.0);

	// ambient
	vec3 texColor = texture(diffuseTexture, texCoord).rgb,
		color = sun.ambient.xyz * texColor;

	float shadow = computeShadow(_normal, lightDir);

	// diffuse
	color += (1.0 - shadow) * diffuseFactor * texColor * sun.color.xyz;
	// specular
	color += (1.0 - shadow) * specularFactor * sun.color.xyz;

	return color;
}

vec3 computePointLight(vec3 _normal, PointLight pLight, vec3 viewDir, vec3 fragPos) {
	// point light calculations
	vec3 lPos = (viewMatrix * vec4(pLight.position, 1.0)).xyz;
	vec3 frag2LightDir = normalize(lPos - fragPos);
	// blinn-phong special vector
	vec3 halfwayDir = normalize(viewDir + frag2LightDir);

	float k1 = 0.09f, kq = 0.032f;
	float dist = distance(lPos, fragPos),
		// for gamma correction, this formula works better
		att = 1.0f / (dist * dist);

	// diffuse
	float diffuseFactor = max(dot(_normal, frag2LightDir), 0);

	// specular
	vec3 reflectDir = reflect(frag2LightDir, _normal);
	float shininess = 32.0, // this one can be set as uniform...
		  specularFactor = pow(max(dot(_normal, halfwayDir), 0.0), shininess);

	// ambient
	vec3 texColor = texture(diffuseTexture, texCoord).rgb,
		color = pLight.color * 0.1 * texColor;
	// diffuse
	color += diffuseFactor * texColor * pLight.color * att;
	// specular
	color += specularFactor * pLight.color * att;

	return color;
}

float computeShadow(vec3 _normal, vec3 lightDir) {
	// perform perspective divide, since this one is not explicitly applied by opengl
	vec3 projCoords = lightFragPos.xyz / lightFragPos.w,
		 projCoords2 = (lightProjMatrix * lightFragPos).xyz;
	
	projCoords2 = projCoords2 / lightFragPos.w;

	// covert to [0, 1] range -> needed for the depthTexture UVs
	projCoords = (projCoords2 * 0.5) + 0.5;

	// retrieve depth values
	float closestDepth = texture(depthTexture, projCoords.xy).r,
		  currentDepth = projCoords.z;

	float bias = max(0.05 * (1.0 - dot(_normal, lightDir)), 0.005),
		  // the bigger the z component is: the closer it is to the camera
		  shadow = (currentDepth - bias) > closestDepth ? 1.0 : 0.0;
	
	return shadow;
}
