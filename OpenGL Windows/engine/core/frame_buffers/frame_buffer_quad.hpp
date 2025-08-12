#pragma once

#include <iostream>

#include <glad/glad.h>

#include "geometry/Mesh.hpp"
#include "frame_buffer.hpp"

namespace CEngine {

	class FrameBufferQuad {
	public:
		// Creates a buffer with given texture size and shader
		FrameBufferQuad(int, int, Shader* const);
		// Creates a buffer with given texture size, shader and custom verts/uvs
		FrameBufferQuad(int, int, Shader* const, float[], u_long);

		void UseAsRenderTarget() const;

		//	Moves rendered texture to the Quad's shader and draws it as a plane 
		//  [NOTE] scene should have been rendered already
		//	@param shouldClearDefaultBuffer = If true: will clear color buffer
		void RenderQuad(bool) const;

	private:

		void initBuffers(int, int, Shader* const);

		void initQuadMesh();
		void initQuadMesh(float[], u_long);
		
		std::unique_ptr<FrameBuffer> framebufferPtr;
		std::unique_ptr<Mesh> quad;
		std::unique_ptr<Material> quadMaterial;
	};
}