#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

// what goes to the fragment shader...
out vec2 texCoord;
out vec3 normal;
out vec4 fragPos;

uniform mat4 model;

// uniform block for common matrices
layout(std140) uniform GlobalMatrices {
	mat4 view;
	mat4 projection;
};

void main() {
	// send output to fragment shader in view space
	texCoord = aTexCoord;
	normal = (view * model * vec4(aNormal, 0.0)).xyz;
	fragPos = view * model * vec4(aPos.xyz, 1.0);

	gl_Position = projection * view * model * vec4(aPos.xyz, 1.0);
}