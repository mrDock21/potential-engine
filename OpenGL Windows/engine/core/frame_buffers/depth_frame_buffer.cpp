#include "depth_frame_buffer.hpp"

using namespace CEngine;

DepthFrameBuffer::DepthFrameBuffer(const unsigned int& with, const unsigned int& height) {

	frameBufferPtr = std::make_shared<FrameBuffer>();
	frameBufferPtr->Use();

	depthTexturePtr = std::make_shared<DepthTexture>(with, height);
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
	frameBufferPtr->Unbind();
	depthTexturePtr->Unbind();
}

void DepthFrameBuffer::Use() const {
	frameBufferPtr->Use();
	depthTexturePtr->Use();
}

void DepthFrameBuffer::Render(std::function<void()> sceneRenderFunc) const {
	int viewportData[4];

	// save previous size
	glGetIntegerv(GL_VIEWPORT, viewportData);

	// and adjust to this framebuffer
	glViewport(0, 0, depthTexturePtr->width, depthTexturePtr->height);

	Use();

	glClear(GL_DEPTH_BUFFER_BIT);

	glEnable(GL_DEPTH_TEST);

	// render scene
	sceneRenderFunc();

	// go back to normal state
	frameBufferPtr->Unbind();
	depthTexturePtr->Unbind();

	glViewport(0, 0, viewportData[2], viewportData[3]);
}

std::shared_ptr<DepthTexture> DepthFrameBuffer::Tex() const {
	return depthTexturePtr;
}

