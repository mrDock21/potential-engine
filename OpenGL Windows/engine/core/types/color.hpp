#pragma once

#include "math_types/vector4.hpp"

namespace CEngine {
	class Color {
	public:
		// to be able to set uniforms in shaders
		friend class Shader;

		// static premade colors
		static Color White, Black, Red, Blue, Green, Transparent;

		Color();
		Color(const Color&);
		Color(const float&, const float&, const float&);
		Color(const float&, const float&, const float&, const float&);

		void R(const float&);
		void G(const float&);
		void B(const float&);
		void A(const float&);

		void RGB(const float&, const float&, const float&);
		void RGBA(const float&, const float&, const float&, const float&);

		void Set255(const Vector3&);
		void Set255(const Vector4&);

		Vector3 RGB() const;
		Vector4 RGBA() const;

		float R() const;
		float G() const;
		float B() const;
		float A() const;

		static Color From255(const float&, const float&, const float&);
		static Color From255(const float&, const float&, const float&, const float&);
		static Color From255(const Vector3&);
		static Color From255(const Vector4&);

		Color& operator = (const Color&);

	private:
		Vector4 color;
	};

}

