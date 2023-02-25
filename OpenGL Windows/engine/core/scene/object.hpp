#pragma once

#include <iostream>

#include "math_types/transform.hpp"

class Object {
public:
	std::string Name;

	Object();
	Object(const std::string&);

private:
	Components::Transform transform;
};