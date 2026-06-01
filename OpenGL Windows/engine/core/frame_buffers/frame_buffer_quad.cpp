#include "frame_buffer_quad.hpp"

using namespace CEngine;

FrameBufferQuad::FrameBufferQuad(int texWidth, int texHeight, const std::shared_ptr<Shader>& s) {

	initBuffers(texWidth, texHeight, s);
	initQuadMesh();

	// bind them together...
	quad->Use();
	quadMaterial->Use();
}

FrameBufferQuad::FrameBufferQuad(
	int texW, int texH, const std::shared_ptr<Shader>& shader, float verts_uvs[], u_long arrSize
) {
	initBuffers(texW, texH, shader);

	initQuadMesh(verts_uvs, arrSize);

	// bind them together...
	quad->Use();
	quadMaterial->Use();
}

void FrameBufferQuad::UseAsRenderTarget() const {

	framebufferPtr->UseAsRenderTarget();
	// clear everything from *this framebuffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
}

void FrameBufferQuad::RenderQuad() const {
	RenderQuad(false);
}

//	Moves rendered texture to the Quad's shader and draws it as a plane 
//  [NOTE] scene should have been rendered already
void FrameBufferQuad::RenderQuad(bool shouldClearDefaultBuffer) const {

	RenderQuad(shouldClearDefaultBuffer, []() { /* Empty lambda */ });
}

void FrameBufferQuad::RenderQuad(bool shouldClear, std::function<void()> inBetweenFunc) const {
	framebufferPtr->Unbind();
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

	if (shouldClear)
		// will delete everything in the screen
		glClear(GL_COLOR_BUFFER_BIT);

	glDisable(GL_DEPTH_TEST);

	quad->Use();
	quadMaterial->Use();
	quadMaterial->Render();
	inBetweenFunc();
	quad->Draw();
}

void FrameBufferQuad::initBuffers(int texWidth, int texHeight, const std::shared_ptr<Shader>& shader) {
	std::shared_ptr<Texture> screenTexture;
	std::shared_ptr<RenderBuffer> stencilDepthBuffer;

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
	quadMaterial = std::make_shared<Material>(shader);
	quadMaterial->AddTexture(screenTexture, "screenTexture");

	// all set: go back to default
	framebufferPtr->Unbind();
}

void FrameBufferQuad::initQuadMesh() {
	// use default quad (whole screen)
	float quadVertices[] = {
		// positions // texCoords
		-1.0f, 1.0f, 0.0f, 1.0f,
		-1.0f,-1.0f, 0.0f, 0.0f,
		1.0f,-1.0f,  1.0f, 0.0f,

		-1.0f, 1.0f, 0.0f, 1.0f,
		1.0f,-1.0f,  1.0f, 0.0f,
		1.0f, 1.0f,  1.0f, 1.0f
	};
	
	initQuadMesh( quadVertices, sizeof(quadVertices) );
}

void FrameBufferQuad::initQuadMesh(float verts[], u_long arrSize) {
	const u_long componentsSize = sizeof(float) * 4;
	const int posAttrIndex(0), uvAttrIndex(1), attrSize(2);
	const bool shouldBeNormalized(false);
	const float uvLocOffset = 2 * sizeof(float);

	quad = std::make_unique<Mesh>(verts, arrSize, componentsSize);

	quad->Use();
	quad->SetAttribute(
		posAttrIndex, attrSize, GL_FLOAT, shouldBeNormalized, componentsSize, 0
	);
	quad->SetAttribute(
		uvAttrIndex, attrSize, GL_FLOAT, shouldBeNormalized, componentsSize, uvLocOffset
	);
}
