#include "frame_buffer.hpp"

using namespace CEngine;

FrameBuffer::FrameBuffer() : texturePtr(nullptr), renderBufferPtr(nullptr) {

	int numberOfBuffers(1);

	glGenFramebuffers(numberOfBuffers, &ID);
}

FrameBuffer::~FrameBuffer() {

	int numberOfBuffers(1);

	glDeleteFramebuffers(numberOfBuffers, &ID);
}

/// <summary>
///		Binds it to OpenGL state machine
/// </summary>
void FrameBuffer::Use() const {
	Use(BindingMode::Both);
}

void FrameBuffer::Use(FrameBuffer::BindingMode mode) const {

	int bindingMode(GL_FRAMEBUFFER);

	if (mode == BindingMode::Read)
		bindingMode = GL_READ_FRAMEBUFFER;
	else if (mode == BindingMode::Draw)
		bindingMode = GL_DRAW_FRAMEBUFFER;

	glBindFramebuffer(bindingMode, ID);
}

void FrameBuffer::Unbind() const {
	// back to default framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

/// <summary>
///		Binds this object as the Read/Draw target.
///		Validates a complete buffer
/// </summary>
void FrameBuffer::UseAsRenderTarget() const {
	
	Use();

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		Logger::Warning(
			"--FrameBuffer is not complete. Unable to use it as render target\n");
		Unbind();
		return;
	}

	// safe to use target...
}

void FrameBuffer::Attach(std::shared_ptr<Texture> texture, FrameBuffer::BindingMode mode) {
	int bindingMode(GL_FRAMEBUFFER), mipMapLevel=0;

	if (mode == BindingMode::Read)
		bindingMode = GL_READ_FRAMEBUFFER;
	else if (mode == BindingMode::Draw)
		bindingMode = GL_DRAW_FRAMEBUFFER;

	if (texture == nullptr) {
		Logger::Warning(
			"--FrameBuffer -> Tried to attach NULL texture\n");
		return;
	}

	Use();
	
	texturePtr = texture;
	texturePtr->Use();

	glFramebufferTexture2D(
		bindingMode, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texturePtr->ID, mipMapLevel
	);
}

void FrameBuffer::Attach(std::shared_ptr<RenderBuffer> renderBuffer) {

	int attachmentType(GL_DEPTH_STENCIL_ATTACHMENT);

	if (renderBuffer == nullptr) {
		Logger::Warning(
			"--FrameBuffer -> Tried to attach NULL render buffer\n");
		return;
	}

	renderBufferPtr = renderBuffer;

	switch ( renderBufferPtr->format )
	{
	case CEngine::RenderBuffer::InternalFormat::Depth16:
	case CEngine::RenderBuffer::InternalFormat::Depth24:
	case CEngine::RenderBuffer::InternalFormat::Depth32:
		attachmentType = GL_DEPTH_ATTACHMENT;
		break;

	case CEngine::RenderBuffer::InternalFormat::Stencil:
		attachmentType = GL_STENCIL_ATTACHMENT;
		break;

	// case CEngine::RenderBuffer::InternalFormat::Color: ToDo* -> GL_COLOR_ATTACHMENTi

	default:
		// both depth and stencil
		break;
	}

	Use();
	renderBufferPtr->Use();

	glFramebufferRenderbuffer(GL_FRAMEBUFFER, attachmentType, GL_RENDERBUFFER, renderBufferPtr->ID);
}

