#include "scene.hpp"

using namespace CEngine;

Scene::Scene() : innerCount(0) {

}

Scene::~Scene() {
	actors.clear();
}

void Scene::AddObject(std::shared_ptr<Object> obj) {
	std::shared_ptr<Object> ptr(obj);
	actors.push_back({ 
		innerCount++, std::move(ptr)
	});

	// all objects must have at least these two in common
	obj->Initialize(globalMatricesBuffer, globalLightsBuffer);
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

void Scene::InitializeGlobalData() {
	
	globalMatricesBuffer = UBO(
		GMATRICES_UBO_NAME,		// same as shader...
		GMATRICES_UBO_INDEX,	//	for global matrices = 0
		0,						
		Matrix4::Size() * 2		// since there are two matrices (view, proj)
	);
}

void Scene::Render(const Matrix4& view, const Matrix4& proj) {

	// update View matrix
	// since it's the first, we set offset to 0
	globalMatricesBuffer.Upload(view, 0);
	
	// update (perspective) Projection matrix
	// since the 'view' matrix is before this one, the offset is different
	globalMatricesBuffer.Upload(proj, Matrix4::Size());
}
