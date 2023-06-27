#include "scene.hpp"

using namespace CEngine;

Scene::Scene() : innerCount(0), sun("sun", Vector3(1, -1, -1)) {

}

Scene::~Scene() {
	actors.clear();
}

void Scene::AddActor(std::shared_ptr<Actor> obj) {
	std::shared_ptr<Actor> ptr(obj);
	actors.push_back({ 
		innerCount++, std::move(ptr)
	});

	obj->material()->SetUniformBlock(uniformBuffer);
}

void Scene::RemoveActor(Actor* obj) {
	
	int index(0);

	for (auto i(actors.begin()); i != actors.end(); i++, index++) {
		if (i->Obj.get() == obj)
			break;
	}
	RemoveActor(index);
}

void Scene::RemoveActor(int index) {
	actors.erase(actors.begin() + index);
}

void Scene::InitializeGlobalData(const std::string& name, const uint& index, const u_long& size) {
	uniformBuffer = UBO(name, index, 0, size);
}

void Scene::Render(const Matrix4& view, const Matrix4& proj) {

	// since it's the first, we set offset to 0
	uniformBuffer.Upload(view, 0); 
	
	// since the 'view' matrix is before this one
	uniformBuffer.Upload(proj, Matrix4::Size());
}
