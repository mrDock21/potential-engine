#include "object.hpp"

Object::Object() : Name("Object"), transform() {

}

Object::Object(const std::string& name) : Name(name), transform() {

}

Object::Object(const std::string& name, const Vector3& pos) : 
	Name(name), transform() {

	transform.SetPosition(pos);
}

Object::Object(const std::string& name, const Vector3& pos, const Vector3& rot) 
	: Name(name), transform() {

	transform.SetPosition(pos);
	transform.SetEulerAngles(rot);
}

Object::Object(
	const std::string& name, 
	const Vector3& pos, 
	const Vector3& rot, 
	const Vector3& scale 
) : Name(name), transform() {

	transform.SetPosition(pos);
	transform.SetEulerAngles(rot);
	transform.SetScale(scale);
}

Components::Transform& Object::Transform() {
	return transform;
}