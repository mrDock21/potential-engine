#include "render_buffer.hpp"

using namespace CEngine;

RenderBuffer::RenderBuffer() : format(InternalFormat::Depth16), width(0), height(0) {
	int numBuffers(1);

	glGenRenderbuffers(numBuffers, &ID);
}

RenderBuffer::RenderBuffer(int w, int h, const InternalFormat& internalFormat) : RenderBuffer() {
	
	CreateStorage(w, h, internalFormat);
}

RenderBuffer::~RenderBuffer() {
	int amountToDelete(1);

	glDeleteRenderbuffers(amountToDelete, &ID);
}

void RenderBuffer::Use() const {
	glBindRenderbuffer(GL_RENDERBUFFER, ID);
}

void RenderBuffer::CreateStorage(int w, int h, const InternalFormat& internalFormat) {
	width = w;
	height = h;

	format = internalFormat;

	Use();

	createStorage();
}

void RenderBuffer::createStorage() {

	int internalFormat(GL_DEPTH24_STENCIL8);

	if (format == InternalFormat::Depth16)
		internalFormat = GL_DEPTH_COMPONENT16;
	else if (format == InternalFormat::Depth24)
		internalFormat = GL_DEPTH_COMPONENT24;
	else if (format == InternalFormat::Depth32)
		internalFormat = GL_DEPTH_COMPONENT32F;
	else if (format == InternalFormat::Stencil)
		internalFormat = GL_STENCIL_INDEX8;
	else if (format == InternalFormat::Depth32Stencil)
		internalFormat = GL_DEPTH32F_STENCIL8;


	glRenderbufferStorage(GL_RENDERBUFFER, internalFormat, width, height);
}
