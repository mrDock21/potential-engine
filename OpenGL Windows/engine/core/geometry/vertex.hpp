
#pragma once

#include "./../math_types/vector3.hpp"
#include "./../math_types/vector2.hpp"

namespace CEngine {

	class Vertex {
	public:

		Vertex();
		Vertex(const Vector3&, const Vector3&, const Vector2&);

		void Pos(const Vector3&);
		void Normal(const Vector3&);
		void UV(const Vector2&);
		
		Vector3 Pos() const;
		Vector3 Normal() const;
		Vector2 UV() const;

	private:
		Vector3 pos, normal;
		Vector2 uv;

	};

}