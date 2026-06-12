#pragma once
#include <iostream>
#include <glad/glad.h>

#include "types/types.hpp"

#include "shading/itexture.hpp"

namespace CEngine {
	
	class DepthTexture : public ITexture {

		// to be able to access the ID
		friend class DepthFrameBuffer;

	public:

		DepthTexture(const unsigned int&, const unsigned int&);
		~DepthTexture();

		virtual void Use() const;
		void Unbind() const;

	private:
		uint ID;
		unsigned int width, height;
	};
}