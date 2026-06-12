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
	meshPtr.reset();
	meshPtr = std::unique_ptr<Mesh>(m);
}

void Actor::SetMaterial(const std::shared_ptr<Material>& material) {
	
	materialPtr = material;

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

void Actor::RenderWith(const std::shared_ptr<Material>& usedMat) {
	// binds VAO
	meshPtr->Render();

	usedMat->Render();

	// set base uniforms (model is for the mesh)
	materialPtr->SetUniform("model", Transform().ModelMatrix());

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

const std::shared_ptr<Material>& Actor::material() const {
	return materialPtr;
}

Mesh* Actor::mesh() const {
	return meshPtr.get();
}
