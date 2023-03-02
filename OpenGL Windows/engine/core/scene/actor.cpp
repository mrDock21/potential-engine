#include "actor.hpp"

Actor::Actor() : Object() { }


Actor::Actor(Mesh* meshPtr, Material* matPtr) 
	: Object(), mesh(meshPtr), material(matPtr) {
}

Actor::Actor(const std::string& name) : Object(name) { }

Actor::Actor(const std::string& name, Mesh* meshPtr, Material* matPtr) 
	: Object(name), mesh(meshPtr), material(matPtr) {
	
}

void Actor::SetMesh(Mesh* m) {
	// immplement OpenGL vertex update (in Mesh class)
}

void Actor::SetMaterial(Material* m) {
	// rebing VAO with new material
}

void Actor::Render(const Matrix4& view, const Matrix4& proj) {
	// draw the entire object at once
	BeginRender(view, proj);
	EndRender();
}

void Actor::BeginRender(const Matrix4& view, const Matrix4& proj) {
	// binds VAO
	mesh->Render();
	// binds shaders & textures
	material->Render();

	// set base uniforms
	material->SetUniform<Matrix4>("view", view);
	material->SetUniform<Matrix4>("projection", proj);
	material->SetUniform<Matrix4>("model", Transform().ModelMatrix());
}

void Actor::EndRender() {
	mesh->Draw();
}