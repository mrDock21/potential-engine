#pragma once
#include <iostream>
#include <glad/glad.h>

#include "types/types.hpp"

namespace CEngine {
	
	class DepthTexture {

		// to be able to access the ID
		friend class DepthFrameBuffer;

	public:

		DepthTexture(const unsigned int&, const unsigned int&);
		~DepthTexture();

		void Use() const;
		void Unbind() const;

	private:
		uint ID;
		unsigned int width, height;
	};
}