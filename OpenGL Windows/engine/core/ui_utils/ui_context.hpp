#pragma once

#include <iostream>
#include <memory>
#include <string>

#include "imgui/imgui.h"

#include "scene/object.hpp"

namespace CEngine {

	class UIContext {
	public:

		virtual void UpdateUI() = 0;
		virtual void Reset() = 0;
		virtual void OnContextChanged(const std::shared_ptr<Object>&) = 0;

		void Context(const std::shared_ptr<Object>& contextPtr) {
			
			context = contextPtr;

			if (context == nullptr) {
				Reset();
				return;
			}

			OnContextChanged(context);
		}

		const std::shared_ptr<Object>& Context() {
			return context;
		}

		bool HasContext() { 
			return context != nullptr; 
		}

		std::string WindowName;

	private:
		std::shared_ptr<Object> context;
	};
}