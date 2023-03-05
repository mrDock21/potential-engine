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
	vec3 position;
	vec4 color, ambient, diffuse, specular;
	float k1, kq;
};
struct Material {
   float shininess;
};

// light and main material
uniform Light light;
uniform Material material;

void main() {
	// diffuse compute
	vec3 lightDir = normalize(light.position - fragPos.xyz);
	float dist = distance(light.position, fragPos.xyz),
		  att = 1.0f / (1.0f + light.k1 * dist + light.kq * (dist * dist));
	float intensity = clamp(dot(lightDir, normal), 0.0, 1.0);

	// specular compute
	vec3 viewDir = normalize(vec3(0) - fragPos.xyz); // from camera to frag
	float specular = pow( clamp( dot( reflect(-lightDir, normal), viewDir), 0.0, 1.0), material.shininess);
	float specularFlag = texture(specularTexture, texCoord).r;

	// emission filter
	vec3 emissionColor = texture(emissionTexture, texCoord).rgb;
	float emissionFlag = step(0.1, emissionColor.r);

	// color combination
	vec3 color, texColor = texture(diffuseTexture, texCoord).rgb;
	// ambient
	color = light.ambient.rgb * texColor * att;
	// diffuse
	color += intensity * texColor * light.color.rgb * att;
	// emission
	color += (1.0 - step(0.1, specularFlag)) * emissionFlag * emissionColor;
	// specular
	color += light.specular.rgb * specularFlag * light.color.rgb * specular * att;
	FragColor = vec4(color, 1.0);
}