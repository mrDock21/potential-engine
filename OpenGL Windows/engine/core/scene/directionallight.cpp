#include "directionallight.hpp"

DirectionalLight::DirectionalLight(const Vector3& pos, const Vector3& dir) {
	
	Transform().SetPosition(pos);
	Dir(dir);
}

void DirectionalLight::Dir(const Vector3& v) {
	dir = v;
}

Vector3 DirectionalLight::Dir() const
{
	return dir;
}
