#version 330 core

out vec4 FragColor;
in vec2 TexCoords;

uniform sampler2D screenTexture;

const float GAMMA = 2.2;

void main() {

	vec4 texColor = texture(screenTexture, TexCoords);

	// GAMMA correction
	FragColor.rgb = pow( texColor.rgb, vec3(1.0 / GAMMA) );
	// do not forget the alpha channel 
	FragColor.a = texColor.a;
}
