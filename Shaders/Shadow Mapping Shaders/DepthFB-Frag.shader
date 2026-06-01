#version 330 core

/**
* This shader is meant to be used by the 3D objects ONLY for the depth buffer texture generation
*/

void main() {
	// The below is automatically done bu OpenGL in the background
	//	uncommenting this would just do the same thing

	// gl_FragDepth = gl_FragCoord.z;
}
