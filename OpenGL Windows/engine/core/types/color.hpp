#pragma once

#include "math_types/vector4.hpp"

class Color {
public:

	Color();
	Color(const Color&);
	Color(const float&, const float&, const float&);

	void R(const float&);
	void G(const float&);
	void B(const float&);
	void A(const float&);

	void RGB(const float&, const float&, const float&);
	void RGBA(const float&, const float&, const float&, const float&);

	Vector3 RGB() const;
	Vector4 RGBA() const;

	float R() const;
	float G() const;
	float B() const;
	float A() const;

	Color& operator = (const Color&);

private:
	Vector4 color;
};