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
struct Light {
	// position should be in view space
	vec3 position, forward;
	vec4 color, ambient, diffuse, specular;
	float k1, kq, maxAngle;
};
struct Material {
	float shininess;
};

// light and main material
uniform Light light;
uniform Material material;

/**
* This shader is meant to be used by the 3D objects, NOT the light itself
*/

void main() {
	// diffuse compute
	// light position should be in VIEW SPACE
	vec3 lightDir = normalize(light.position - fragPos.xyz);
	
	float k1 = 0.09f, kq = 0.032f; // since there is no point light here

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
	color = light.ambient.rgb * texColor * att * spotlightFlag;
	// diffuse
	color += intensity * texColor * light.color.rgb * att * spotlightFlag;
	// emission
	color += (1.0 - step(0.1, specularFlag)) * emissionFlag * emissionColor;
	// specular
	color += light.specular.rgb * specularFlag * light.color.rgb * 
			 specular * att * spotlightFlag;

	// debug
	float r = distance(fragPos.xy, vec2(0, 0));
	if (r <= 0.05) {
		color.rgb = vec3(1);
	}

	FragColor = vec4(color, 1.0);
}