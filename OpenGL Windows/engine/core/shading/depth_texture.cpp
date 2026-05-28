#include "depth_texture.hpp"

using namespace CEngine;

DepthTexture::DepthTexture(const unsigned int& texWidth, const unsigned int& texHeight) {
	
	width = texWidth;
	height = texHeight;

	glGenTextures(1, &ID);
	Use();
	glTexImage2D(
		GL_TEXTURE_2D,		// target
		0,					// level
		GL_DEPTH_COMPONENT,	// internal format
		width,
		height,
		0,					//border
		GL_DEPTH_COMPONENT,	// component
		GL_FLOAT,			// data type
		NULL				// pixels data
	);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
}

void DepthTexture::Use() const {
	glBindTexture(GL_TEXTURE_2D, ID);
}

DepthTexture::~DepthTexture() {
	glDeleteTextures(1, &ID);
}