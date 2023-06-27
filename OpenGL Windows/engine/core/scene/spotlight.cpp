#include "spotlight.hpp"

using namespace CEngine;

SpotLight::SpotLight() {

}

SpotLight::SpotLight(const float& angle) : spotAngle(angle) {
	
}

void SpotLight::Forward(const Vector3& dir) {
	Transform().Forward(dir);
}

void SpotLight::SpotAngle(const float& angle) {
	spotAngle = angle;
}

float SpotLight::SpotAngle() const {
	return spotAngle;
}