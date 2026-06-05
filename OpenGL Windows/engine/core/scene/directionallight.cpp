#include "directionallight.hpp"

using namespace CEngine;

DirectionalLight::DirectionalLight() {
	SetColor( Color::White );
	SetPosition( Vector3::Zero() );
	SetRotation( Vector3::Zero() );

}

DirectionalLight::DirectionalLight(const Vector3& pos, const Vector3& eulers) {

	SetColor( Color::White );
	SetPosition( pos );
	SetRotation( eulers );
}

DirectionalLight::DirectionalLight(const Color& color, const Vector3& pos, const Vector3& eulers)
	: DirectionalLight(pos, eulers) {

	SetColor(color);
}

Vector3 DirectionalLight::Dir() {
	return Transform().Forward();
}

const DirectionalLight& DirectionalLight::operator = (const DirectionalLight& dl) {
	

	SetPosition( dl.GetPosition() );

	dir = dl.dir;
	color = dl.color;

	return *this;
}