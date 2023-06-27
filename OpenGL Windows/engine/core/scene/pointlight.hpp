#pragma once

#include "light.hpp"
#include "types/color.hpp"

namespace CEngine {

	class PointLight : public Light {
	public:
		PointLight();
		PointLight(const float&, const float&, const float&);
		PointLight(const float&, const float&, const float&, const Color&);

		void K1(const float&);
		void Kq(const float&);
		void Distance(const float&);
	
		float K1() const;
		float Kq() const;
		float Distance() const;

	private:
		float k1, kq, distance;
	};
}
