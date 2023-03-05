#include "pointlight.hpp"

PointLight::PointLight() { }

PointLight::PointLight(const float& k1, const float& kq, const float& dist) {
	K1(k1); Kq(kq); Distance(dist);
}

PointLight::PointLight(const float& k1, const float& kq, const float& dist, const Color& c) 
	: PointLight(k1, kq, dist) {
	
	SetColor(c);
}

void PointLight::K1(const float& value) {
	k1 = value;
}

void PointLight::Kq(const float& value) {
	kq = value;
}

void PointLight::Distance(const float& value) {
	distance = value;
}

float PointLight::K1() const {
	return k1;
}

float PointLight::Kq() const {
	return kq;
}

float PointLight::Distance() const {
	return distance;
}
