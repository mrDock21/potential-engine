#pragma once

#include <iostream>

#include <glad/glad.h>

#include "types/types.hpp"

namespace CEngine {

	class RenderBuffer {
	public:

		enum class InternalFormat { 
			Depth16, Depth24, Depth32, Stencil, Depth24Stencil, Depth32Stencil
		};

		// Creates buffer 
		RenderBuffer();
		// Creates buffer and allocates storage
		RenderBuffer(int, int, const InternalFormat&);
		~RenderBuffer();

		void Use() const;

		void CreateStorage(int, int, const InternalFormat&);

		friend class FrameBuffer;

	private:

		void createStorage();

		uint ID;
		int width, height;
		InternalFormat format;
	};
}