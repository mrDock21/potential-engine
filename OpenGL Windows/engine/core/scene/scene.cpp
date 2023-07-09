#include "scene.hpp"

using namespace CEngine;

Scene::Scene() : innerCount(0), lightCount(0) {
	Color c;
	c.Set255(Vector3(255.0f, 87.0f, 51.0f));
	sun.SetColor(c);
	sun.Dir(Vector3(-1, -1, -1));
}

Scene::~Scene() {
	actors.clear();
}

void Scene::AddPointLight(std::shared_ptr<PointLight> plight) {
	
	lights[lightCount] = plight.get();
	lightCount++;

	AddObject(plight);
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

	globalLightsBuffer = UBO(
		GLIGHTS_UBO_NAME,
		GLIGHTS_UBO_INDEX,
		0,
		LIGHTS_ARRAY_SIZE	+		// PointLight struct size + array size
		SUN_STRUCT_SIZE		+		// SunLight struct size
		sizeof(float)				// scalar of "Num Lights"
	);
}

void Scene::Render(const Matrix4& view, const Matrix4& proj) {

	// update View matrix
	// since it's the first, we set offset to 0
	globalMatricesBuffer.Upload(view, 0);
	
	// update (perspective) Projection matrix
	// since the 'view' matrix is before this one, the offset is different
	globalMatricesBuffer.Upload(proj, Matrix4::Size());

	// update the rest of lights
	UpdateLights();
	UpdateSun();

	for (int i(0); i < actors.size(); i++) {
		actors.at(i).Obj->Render();
	}
}

std::shared_ptr<Object> Scene::GetChild(const int& i) {
	return actors.at(i).Obj;
}

void Scene::UpdateLights() {
	
	float offset(0);

	for (int i(0); i < lightCount; i++) {
		// set position
		globalLightsBuffer.Upload(lights[i]->Transform().Position(), offset);
		// skip to the next property (+ skip one float)...
		offset += Vector4::Size();
		// set color
		globalLightsBuffer.Upload(lights[i]->GetColor().RGBA(), offset);
		// proceed to next one...
		offset += Vector4::Size();
	}
}

void Scene::UpdateSun() {
	// since the array is allocated in memory already, we skip the whole thing...
	float offset = LIGHTS_ARRAY_SIZE;
	//	upload dir
	globalLightsBuffer.Upload(Vector4(sun.Dir(), 0.0f), offset);
	offset += Vector4::Size();
	//	upload color
	globalLightsBuffer.Upload(sun.GetColor().RGBA(), offset);
	offset += Vector4::Size();
	//	upload ambient color
	globalLightsBuffer.Upload(sun.GetColor().RGBA() * 0.1f, offset);
	offset += Vector4::Size();
	// update current count
	globalLightsBuffer.Upload((float)lightCount, offset);
}

