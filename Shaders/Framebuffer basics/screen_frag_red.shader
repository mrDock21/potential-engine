#version 330 core

out vec4 FragColor;
in vec2 TexCoords;

uniform sampler2D screenTexture;

// this is just for debugging. Shows everything as red

void main() {
	vec3 col = texture(screenTexture, TexCoords).rgb * vec3(1, 0, 0);
	FragColor = vec4(1.0, 0.0, 0.0, 1.0);
}
