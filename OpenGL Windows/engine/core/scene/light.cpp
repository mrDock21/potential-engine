#include "light.hpp"

void Light::SetColor(const Color& c) {
	color = c;
}

Color Light::GetColor() const {
	return color;
}
