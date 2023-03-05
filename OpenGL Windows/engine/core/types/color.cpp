#include "color.hpp"

Color Color::White = Color(1, 1, 1);
Color Color::Black = Color(0, 0, 0);
Color Color::Red = Color(1, 0, 0);
Color Color::Green = Color(0, 1, 0);
Color Color::Blue = Color(0, 0, 1);
Color Color::Transparent = Color(0, 0, 0, 0);

Color::Color() : color() { A(1.0f); }

Color::Color(const Color& other) : color(other.color) { }

Color::Color(const float& r, const float& g, const float& b) {
	RGB(r, g, b); A(1.0f);
}

Color::Color(const float& r, const float& g, const float& b, const float& a) 
	: Color(r, g, b) {
	A(a);
}

void Color::R(const float& r) {
	color.SetX(r);
}

void Color::G(const float& g) {
	color.SetY(g);
}

void Color::B(const float& b) {
	color.SetZ(b);
}

void Color::A(const float& a) {
	color.SetW(a);
}

void Color::RGB(const float& r, const float& g, const float& b) {
	R(r); G(g); B(b);
}

void Color::RGBA(const float& r, const float& g, const float& b, const float& a) {
	RGB(r, g, b); A(a);
}

Vector3 Color::RGB() const {
	return color.ToVector3();
}

Vector4 Color::RGBA() const {
	return color;
}

float Color::R() const {
	return color.X();
}

float Color::G() const {
	return color.Y();
}

float Color::B() const {
	return color.Z();
}

float Color::A() const {
	return color.W();
}

Color& Color::operator=(const Color& other)
{
	color = other.color;
	return *this;
}
