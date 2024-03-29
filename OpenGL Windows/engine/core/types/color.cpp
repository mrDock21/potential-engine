#include "color.hpp"

using namespace CEngine;

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

void Color::Set255(const Vector3& v) {
	color = Vector4(v.X(), v.Y(), v.Z(), 1.0f) / 255.0f;
}

void Color::Set255(const Vector4& v) {
	color = v / 255.0f;
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

Color Color::From255(const float& r, const float& g, const float& b) {
	return From255(Vector3(r, g, b) / 255.0f);
}

Color Color::From255(const float& r, const float& g, const float& b, const float& a) {
	return From255(Vector4(r, g, b, a) / 255.0f);
}

Color Color::From255(const Vector3& v) {
	Color result;
	result.Set255(v);
	return result;
}

Color Color::From255(const Vector4& v) {
	Color result;
	result.Set255(v);
	return result;
}

Color& Color::operator=(const Color& other)
{
	color = other.color;
	return *this;
}
