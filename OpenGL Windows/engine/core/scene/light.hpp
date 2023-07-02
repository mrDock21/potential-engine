#pragma once

#include <iostream>

#include "scene/object.hpp"
#include "types/color.hpp"

namespace CEngine {

	class Light : public Object {
	public:
		// reuse the other constructors...
		using Object::Object;

		void SetColor(const Color&);

		virtual void Initialize(const UBO& matricesUBO, const UBO& lightsUBO) {
			/* Not used for now */
		}

		virtual void Render(){ /* Not used for now */ }

		Color GetColor() const;

	private:
		Color color;
	};
}
