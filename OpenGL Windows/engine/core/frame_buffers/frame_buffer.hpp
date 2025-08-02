#pragma once

#include <iostream>

#include <glad/glad.h>

#include "shading/Texture.hpp"
#include "render_buffer.hpp"

namespace CEngine {

	/// <summary>
	///		Frame Buffer Object
	/// </summary>
	class FrameBuffer {
	public:

		enum class BindingMode{ Read, Draw, Both};

		FrameBuffer();
		~FrameBuffer();

		void Use() const;
		void Use(BindingMode) const;

		// Binds OpenGL's default frame buffer
		void Unbind() const;

		void UseAsRenderTarget() const;

		void Attach(std::shared_ptr<Texture> texture, FrameBuffer::BindingMode mode);
		void Attach(std::shared_ptr<RenderBuffer>);
	private:

		uint ID;
		std::shared_ptr<Texture> texturePtr;
		std::shared_ptr<RenderBuffer> renderBufferPtr;
	};
}
// ToDo: Frame buffer with plane in screen
