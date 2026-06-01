#pragma once

#include <iostream>
#include <functional>

#include <glad/glad.h>

#include "geometry/Mesh.hpp"
#include "frame_buffer.hpp"

namespace CEngine {

	class FrameBufferQuad {
	public:
		// Creates a buffer with given texture size and shader
		FrameBufferQuad(int, int, const std::shared_ptr<Shader>&);
		// Creates a buffer with given texture size, shader and custom verts/uvs
		FrameBufferQuad(int, int, const std::shared_ptr<Shader>&, float[], u_long);

		void UseAsRenderTarget() const;

		void RenderQuad() const;
		//	Moves rendered texture to the Quad's shader and draws it as a plane 
		//  [NOTE] scene should have been rendered already
		//	@param shouldClearDefaultBuffer = If true: will clear color buffer
		void RenderQuad(bool) const;

		/// <summary>
		///		Renders the Quad with a function being called before drawing
		///		in case something needs to be uploaded to the shaders
		/// </summary>
		/// @param shouldClearDefaultBuffer = If true: will clear color buffer
		/// @param shaderInBetweenFunction = Lambda called before drawing [MUST NOT BE NULL]
		void RenderQuad(bool, std::function<void()>) const;

	private:

		void initBuffers(int, int, const std::shared_ptr<Shader>&);

		void initQuadMesh();
		void initQuadMesh(float[], u_long);
		
		std::unique_ptr<FrameBuffer> framebufferPtr;
		std::unique_ptr<Mesh> quad;
		std::shared_ptr<Material> quadMaterial;
	};
}