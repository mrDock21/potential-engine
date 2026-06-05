#include "object.hpp"

using namespace CEngine;

Object::Object() : Name("Object"), transform() {

}

Object::Object(const std::string& name) : Name(name), transform() {

}

Object::Object(const std::string& name, const Vector3& pos) : 
	Name(name), transform() {

	transform.Position(pos);
}

Object::Object(const std::string& name, const Vector3& pos, const Vector3& rot) 
	: Name(name), transform() {

	transform.Position(pos);
	transform.EulerAngles(rot);
}

Object::Object(
	const std::string& name, 
	const Vector3& pos, 
	const Vector3& rot, 
	const Vector3& scale 
) : Name(name), transform() {

	transform.Position(pos);
	transform.EulerAngles(rot);
	transform.Scale(scale);
}

const CEngine::Transform& Object::Transform() {
	return transform;
}

void Object::SetPosition(const Vector3& pos) {
	transform.Position(pos);
}

void Object::MovePosition(const Vector3& dir) {
	transform.Position(
		transform.Position() + dir
	);
}

void Object::SetRotation(const Vector3& eulers) {
	transform.EulerAngles(eulers);
}


void Object::SetRotation(const Quaternion& rot) {
	transform.Rotation(rot);
}

void Object::RotateX(const float& angle) {
	transform.Rotate(angle, transform.Right());
}

void Object::RotateY(const float& angle) {
	transform.Rotate(angle, transform.Up());
}

void Object::RotateZ(const float& angle) {
	transform.Rotate(angle, transform.Forward());
}

void Object::RotateAxis(const float& angle, const Vector3& axis) {
	transform.Rotate(angle, axis);
}

void Object::Rotate(const Quaternion& rot) {
	transform.Rotate(rot);
}


void Object::SetScale(const float& uniformScale) {
	transform.Scale(uniformScale);
}


void Object::SetScale(const Vector3& scaleXYZ) {
	transform.Scale(scaleXYZ);
}

void Object::SetForward(const Vector3& dir) {
	transform.Forward(dir);
}

Vector3 Object::GetPosition() const {
	return transform.Position();
}

Vector3 Object::GetRotation() const {
	return transform.EulerAngles();
}
