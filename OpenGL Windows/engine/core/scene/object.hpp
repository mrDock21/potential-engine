#pragma once

#include <iostream>
#include <vector>

#include "iscene_renderable.hpp"

#include "math_types/transform.hpp"

namespace CEngine {

	/**
	* Represents an empty object in a scene
	*/
	class Object : public ISceneRenderable {
	public:
		std::string Name;

		Object();
		Object(const std::string&);
		Object(const std::string&, const Vector3&);
		Object(const std::string&, const Vector3&, const Vector3&);
		Object(const std::string&, const Vector3&, const Vector3&, const Vector3&);

		const CEngine::Transform& Transform();

		void SetPosition(const Vector3&);
		void MovePosition(const Vector3&);
		void SetRotation(const Vector3&);
		void SetRotation(const Quaternion&);
		void RotateX(const float&);
		void RotateY(const float&);
		void RotateZ(const float&);
		void RotateAxis(const float&, const Vector3&);
		void Rotate(const Quaternion&);
		void SetScale(const float&);
		void SetScale(const Vector3&);

		void SetForward(const Vector3&);

		Vector3 GetPosition() const;
		Vector3 GetRotation() const;

	private:
		CEngine::Transform transform;
	};
}
