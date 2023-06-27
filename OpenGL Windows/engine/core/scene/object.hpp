#pragma once

#include <iostream>
#include <vector>

#include "math_types/transform.hpp"

namespace CEngine {

	/**
	* Represents an empty object in a scene
	*/
	class Object {
	public:
		std::string Name;

		Object();
		Object(const std::string&);
		Object(const std::string&, const Vector3&);
		Object(const std::string&, const Vector3&, const Vector3&);
		Object(const std::string&, const Vector3&, const Vector3&, const Vector3&);

		CEngine::Transform& Transform();

	private:
		CEngine::Transform transform;
	};
}
