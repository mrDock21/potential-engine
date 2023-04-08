#include "actor.hpp"

Actor::Actor() : Object() { }


Actor::Actor(Mesh* meshPtr, Material* matPtr) 
	: Object(), mesh(meshPtr), materialPtr(matPtr) {
}

Actor::Actor(const std::string& name) : Object(name) { }

Actor::Actor(const std::string& name, Mesh* meshPtr, Material* matPtr) 
	: Object(name), mesh(meshPtr), materialPtr(matPtr) {
	
}

void Actor::SetMesh(Mesh* m) {
	// immplement OpenGL vertex update (in Mesh class)
}

void Actor::SetMaterial(Material* m) {
	// rebing VAO with new material
}

void Actor::Render() {
	// draw the entire object at once
	BeginRender();
	EndRender();
}

void Actor::BeginRender() {
	// binds VAO
	mesh->Render();
	// binds shaders & textures
	materialPtr->Render();

	// set base uniforms
	materialPtr->SetUniform("model", Transform().ModelMatrix());
}

void Actor::EndRender() {
	mesh->Draw();
}

Material* Actor::material() const {
	return materialPtr.get();
}