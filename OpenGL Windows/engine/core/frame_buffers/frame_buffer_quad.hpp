#pragma once

#include <iostream>

#include <glad/glad.h>

#include "geometry/Mesh.hpp"
#include "frame_buffer.hpp"

namespace CEngine {

	class FrameBufferQuad {
	public:
		FrameBufferQuad(int, int, Shader* const);

		void UseAsRenderTarget() const;

		//	Moves rendered texture to the Quad's shader and draws it as a plane 
		//  [NOTE] scene should have been rendered already
		void RenderQuad() const;

	private:

		void initBuffers(int, int, Shader* const);

		void initQuadMesh();

		std::unique_ptr<FrameBuffer> framebufferPtr;
		std::unique_ptr<Mesh> quad;
		std::unique_ptr<Material> quadMaterial;
	};
}
