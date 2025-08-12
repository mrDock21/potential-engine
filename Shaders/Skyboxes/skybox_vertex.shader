#version 330 core

layout(location = 0) in vec3 aPos;

out vec3 TexCoords;

// uniform block for common matrices
//	binding Index = 0 (same as Scene.hpp)
layout(std140) uniform GlobalMatrices {
	mat4 view;
	mat4 projection;
};

void main() {
	mat3 view2 = mat3(view);
	TexCoords = aPos;
	vec4 pos = projection * mat4(view2) * vec4(aPos,1.0);
	gl_Position = pos.xyww;
}
