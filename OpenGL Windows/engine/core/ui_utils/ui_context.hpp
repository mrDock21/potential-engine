#pragma once

#include <iostream>
#include <memory>

#include "imgui/imgui.h"

#include "scene/object.hpp"

namespace CEngine {

	class UIContext {
	public:

		virtual void UpdateUI() = 0;
		virtual void Reset() = 0;
		virtual void OnContextChanged(std::shared_ptr<Object>) = 0;

		void Context(std::shared_ptr<Object> contextPtr) {
			
			context = contextPtr;

			if (context == nullptr) {
				Reset();
				return;
			}

			OnContextChanged(context);
		}

		std::shared_ptr<Object> Context() {
			return context;
		}

		bool HasContext() { 
			return context != nullptr; 
		}
	private:
		std::shared_ptr<Object> context;
	};
}