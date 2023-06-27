#include "light.hpp"

using namespace CEngine;

void Light::SetColor(const Color& c) {
	color = c;
}

Color Light::GetColor() const {
	return color;
}
