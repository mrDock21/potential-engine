#pragma once

#include <vector>

#include "ui_context.hpp"

namespace CEngine {

	class UITransform : public UIContext {
	public:

		UITransform();
		virtual void UpdateUI();
		virtual void Reset();
		virtual void OnContextChanged(std::shared_ptr<Object>);

	private:
		std::vector<float> pos, rot, scale;
		Transform *transform;
	};
}