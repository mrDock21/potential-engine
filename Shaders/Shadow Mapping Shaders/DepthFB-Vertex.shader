#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

uniform mat4 model;

// uniform block for common matrices
//	binding Index = 0 (same as Scene.hpp)
//		In this case: the below should be the directional light's matrices
layout(std140) uniform GlobalMatrices {
	mat4 view;
	mat4 projection;
};

void main() {
	// depth buffer only cares about the position
	gl_Position = projection * view * model * vec4(aPos.xyz, 1.0);
}