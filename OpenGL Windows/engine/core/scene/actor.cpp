#include "actor.hpp"

Actor::Actor() : Object() { }

Actor::Actor(Mesh*, Material*) {

}

Actor::Actor(const std::string& name, Mesh* meshPtr, Material* matPtr) 
	: Object(name), mesh(meshPtr), material(matPtr) {
	
}

void Actor::Render(const Matrix4& view, const Matrix4& proj) {

}
