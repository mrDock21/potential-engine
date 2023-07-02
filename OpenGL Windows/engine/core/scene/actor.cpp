#include "actor.hpp"

using namespace CEngine;

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

// From renderable interface...
void Actor::Initialize(const UBO& globalMatrices, const UBO& globalLights) {

	// set both, since lights affect meshes...
	materialPtr->SetUniformBlock(globalMatrices);
	materialPtr->SetUniformBlock(globalLights);
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

	// set base uniforms (model is for the mesh)
	materialPtr->SetUniform("model", Transform().ModelMatrix());
}

void Actor::EndRender() {
	mesh->Draw();
}

Material* Actor::material() const {
	return materialPtr.get();
}