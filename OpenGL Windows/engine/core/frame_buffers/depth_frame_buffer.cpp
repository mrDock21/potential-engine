#include "depth_frame_buffer.hpp"

using namespace CEngine;

DepthFrameBuffer::DepthFrameBuffer(const unsigned int& with, const unsigned int& height) {

	frameBufferPtr = std::make_shared<FrameBuffer>();
	depthTexturePtr = std::make_shared<DepthTexture>(with, height);

	frameBufferPtr->Use();
	depthTexturePtr->Use();

	// linke the texture to this framebuffer
	glFramebufferTexture2D(
		GL_FRAMEBUFFER, 
		GL_DEPTH_ATTACHMENT,	// attachment type
		GL_TEXTURE_2D,			// tex type 
		depthTexturePtr->ID, 
		0						// level
	);

	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);

	//unbind
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

DepthFrameBuffer::~DepthFrameBuffer() {

}

void DepthFrameBuffer::Use() const {

}
