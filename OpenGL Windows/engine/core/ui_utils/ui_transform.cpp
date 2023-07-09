#include "ui_transform.hpp"

using namespace CEngine;

UITransform::UITransform() {
	Context(nullptr);
}

void UITransform::UpdateUI() {
	bool hasContext = HasContext();
	
	// Create a window called "Imported objects" and append into it.
	ImGui::Begin("Transform");
	ImGui::DragFloat3("Position: ", pos.data());
	ImGui::DragFloat3("Rotation: ", rot.data());
	ImGui::DragFloat3("Scale:    ", scale.data());

	if (!hasContext)
		ImGui::Text("None Selected... ");

	ImGui::End();

	if (hasContext) {
		// update the element...
		transform->Position(Vector3(pos[0], pos[1], pos[2]));
		transform->EulerAngles(Vector3(rot[0], rot[1], rot[2]));
		transform->Scale(Vector3(scale[0], scale[1], scale[2]));
	}
}

void UITransform::Reset() {
	pos = rot = { 0, 0, 0 };
	scale = { 1, 1, 1 };
}

void UITransform::OnContextChanged(std::shared_ptr<Object> newContext) {
	Vector3 v;

	transform = &newContext->Transform();
	
	v = transform->Position();
	pos = { v.X(), v.Y(), v.Z() };

	v = transform->EulerAngles();
	rot = { v.X(), v.Y(), v.Z() };
	
	v = transform->Scale();
	scale = { v.X(), v.Y(), v.Z() };
}