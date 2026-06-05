#pragma once
#include <iostream>
#include <functional>
#include <glad/glad.h>

#include "types/types.hpp"

#include "frame_buffer.hpp"
#include "shading/depth_texture.hpp"

namespace CEngine {

	class DepthFrameBuffer {

	public:

		DepthFrameBuffer(const unsigned int&, const unsigned int&);

		void Use() const;
		void UseDepthTextureInShader() const;
		void UseDepthTextureInShader(const int&) const;

		void Render(std::function<void()>) const;

		const std::shared_ptr<DepthTexture>& Tex() const;

	private:
		std::shared_ptr<FrameBuffer> frameBufferPtr;
		std::shared_ptr<DepthTexture> depthTexturePtr;
	};
}