#pragma once

#include "light.hpp"

namespace CEngine {

	class SpotLight : public Light {
	public:

		using Light::Light;

		SpotLight();
		SpotLight(const float&);

		void Forward(const Vector3&);
		void SpotAngle(const float&);

		float SpotAngle() const;

	private:
		float spotAngle;
	};
}
