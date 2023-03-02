#pragma once

#include "scene/light.hpp"
#include "math_types/vector3.hpp"

class DirectionalLight : public Light {
public:
	using Light::Light;

	DirectionalLight(const Vector3&, const Vector3&);

	void Dir(const Vector3&);

	Vector3 Dir() const;

private:
	Vector3 dir;
};