#pragma once

#include "scene/light.hpp"
#include "math_types/vector3.hpp"

namespace CEngine {

	class DirectionalLight : public Light {
	public:
		using Light::Light;

		DirectionalLight();
		DirectionalLight(const Vector3&, const Vector3&);
		DirectionalLight(const Color&, const Vector3&, const Vector3&);

		Vector3 Dir();

		const DirectionalLight& operator = (const DirectionalLight&);

	private:
		Vector3 dir;
	};
}