#include "ui_transform.hpp"

using namespace CEngine;

UITransform::UITransform() : pos(), rot(), scale() {

}

void UITransform::UpdateUI() {
	bool hasContext = HasContext();
	auto obj = Context();

	// Create a window called "Imported objects" and append into it.
	ImGui::Begin( ("Transform: " + WindowName).c_str() );
	ImGui::DragFloat3("Position: ", pos.data());
	ImGui::DragFloat3("Rotation: ", rot.data());
	ImGui::DragFloat3("Scale:    ", scale.data());

	if (!hasContext)
		ImGui::Text("None Selected... ");

	ImGui::End();

	if (hasContext) {
		// update the element...
		obj->SetPosition(Vector3(pos[0], pos[1], pos[2]));
		obj->SetRotation(Vector3(rot[0], rot[1], rot[2]));
		obj->SetScale(Vector3(scale[0], scale[1], scale[2]));
	}
}

void UITransform::Reset() {
	pos = rot = { 0, 0, 0 };
	scale = { 1, 1, 1 };
}

void UITransform::OnContextChanged(const std::shared_ptr<Object>& newContext) {
	Vector3 v;
	
	v = newContext->Transform().Position();
	pos = { v.X(), v.Y(), v.Z() };

	v = newContext->Transform().EulerAngles();
	rot = { v.X(), v.Y(), v.Z() };
	
	v = newContext->Transform().Scale();
	scale = { v.X(), v.Y(), v.Z() };
}