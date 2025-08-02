#include "frame_buffer_quad.hpp"

using namespace CEngine;

FrameBufferQuad::FrameBufferQuad(int texWidth, int texHeight, Shader* const screenShader) {

	initBuffers(texWidth, texHeight, screenShader);
	initQuadMesh();

	// bind them together...
	quad->Use();
	quadMaterial->Use();
}

void FrameBufferQuad::UseAsRenderTarget() const {

	framebufferPtr->UseAsRenderTarget();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
}

//	Moves rendered texture to the Quad's shader and draws it as a plane 
//  [NOTE] scene should have been rendered already
void FrameBufferQuad::RenderQuad() const {

	framebufferPtr->Unbind();

	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	glDisable(GL_DEPTH_TEST);

	quad->Use();
	quadMaterial->Use();
	quadMaterial->Render();
	quad->Draw();
}

void FrameBufferQuad::initBuffers(int texWidth, int texHeight, Shader* const screenShader) {
	std::shared_ptr<Texture> screenTexture;
	std::shared_ptr<RenderBuffer> stencilDepthBuffer;
	std::shared_ptr<Shader> screenShrPtr(screenShader);

	framebufferPtr = std::make_unique<FrameBuffer>();
	framebufferPtr->Use();

	// create and attach color texture
	screenTexture = std::make_shared<Texture>(
		texWidth, texHeight, Texture::TexFormat::RGB, Texture::PixelFormat::RGB
	);
	screenTexture->Use();

	framebufferPtr->Attach(screenTexture, FrameBuffer::BindingMode::Both);

	// create and attach renderbuffer for depth and stencil
	stencilDepthBuffer = std::make_shared<RenderBuffer>(
		texWidth, texHeight, RenderBuffer::InternalFormat::Depth24Stencil
	);
	stencilDepthBuffer->Use();

	framebufferPtr->Attach(stencilDepthBuffer);

	// link material with the empty (screen) texture
	quadMaterial = std::make_unique<Material>(screenShrPtr);
	quadMaterial->AddTexture(screenTexture, "screenTexture");

	// all set: go back to default
	framebufferPtr->Unbind();
}

void FrameBufferQuad::initQuadMesh() {

	float quadVertices[] = {
		// positions // texCoords
		-1.0f, 1.0f, 0.0f, 1.0f,
		-1.0f,-1.0f, 0.0f, 0.0f,
		1.0f,-1.0f,  1.0f, 0.0f,

		-1.0f, 1.0f, 0.0f, 1.0f,
		1.0f,-1.0f,  1.0f, 0.0f,
		1.0f, 1.0f,  1.0f, 1.0f
	};
	const u_long componentsSize = sizeof(float) * 4;
	const int posAttrIndex(0), uvAttrIndex(1), attrSize(2);
	const bool shouldBeNormalized(false);
	const float uvLocOffset = 2 * sizeof(float);

	quad = std::make_unique<Mesh>(quadVertices, sizeof(quadVertices), componentsSize);

	quad->Use();
	quad->SetAttribute(
		posAttrIndex, attrSize, GL_FLOAT, shouldBeNormalized, componentsSize, 0
	);
	quad->SetAttribute(
		uvAttrIndex, attrSize, GL_FLOAT, shouldBeNormalized, componentsSize, uvLocOffset
	);
}
