#pragma once

#include <iostream>

#include "scene/object.hpp"
#include "types/color.hpp"

class Light : public Object {
public:
	// reuse the other constructors...
	using Object::Object;

	void SetColor(const Color&);

	Color GetColor() const;

private:
	Color color;
};