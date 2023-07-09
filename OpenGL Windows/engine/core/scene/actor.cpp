#include "actor.hpp"

using namespace CEngine;

Actor::Actor() : Object() { }


Actor::Actor(Mesh* mPtr, Material* matPtr) 
	: Object(), meshPtr(mPtr), materialPtr(matPtr) {
}

Actor::Actor(const std::string& name) : Object(name) { }

Actor::Actor(const std::string& name, Mesh* mPtr, Material* matPtr) 
	: Object(name), meshPtr(mPtr), materialPtr(matPtr) {
	
}

void Actor::SetMesh(Mesh* m) {
	// immplement OpenGL vertex update (in Mesh class)
}

void Actor::SetMaterial(Material* m) {
	// delete the older one
	materialPtr.reset();
	materialPtr = std::unique_ptr<Material>(m);

	// rebind VAO with new material
	meshPtr->Use();
	materialPtr->Use();
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
	meshPtr->Render();
	// binds shaders & textures
	materialPtr->Render();

	// set base uniforms (model is for the mesh)
	materialPtr->SetUniform("model", Transform().ModelMatrix());
}

void Actor::EndRender() {
	meshPtr->Draw();
}

Material* Actor::material() const {
	return materialPtr.get();
}

Mesh* Actor::mesh() const {
	return meshPtr.get();
}
