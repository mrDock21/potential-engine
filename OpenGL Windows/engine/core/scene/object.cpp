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

CEngine::Transform& Object::Transform() {
	return transform;
}