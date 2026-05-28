#pragma once
#include <iostream>
#include <glad/glad.h>

#include "types/types.hpp"

#include "frame_buffer.hpp"
#include "shading/depth_texture.hpp"

namespace CEngine {

	class DepthFrameBuffer {

	public:

		DepthFrameBuffer(const unsigned int&, const unsigned int&);
		~DepthFrameBuffer();

		void Use() const;

		//public void Render

	private:
		std::shared_ptr<FrameBuffer> frameBufferPtr;
		std::shared_ptr<DepthTexture> depthTexturePtr;
	};
}