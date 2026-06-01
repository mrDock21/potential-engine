#version 330 core

out vec4 FragColor;
in vec2 TexCoords;

uniform sampler2D screenTexture;

const float GAMMA = 2.2;

void main() {
	// since depth is grayscale, just use the same for all rgb values
	vec3 depthValue = vec3(texture(screenTexture, TexCoords).r);
	// GAMMA correction
	FragColor = vec4( pow(depthValue.rgb, vec3(1.0 / GAMMA)), 1.0);
}
