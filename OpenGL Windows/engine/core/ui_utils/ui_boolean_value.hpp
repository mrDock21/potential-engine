#pragma once

#include <functional>
#include <string>

#include "ui_context.hpp"

namespace CEngine {

	class UIBooleanValue : public UIContext {
	public:

		UIBooleanValue() : value(false), onPress(nullptr) { }

		void Attach(std::string _valueName, std::function<void()> callback) {
			valueName = _valueName;
			onPress = callback;
		}

		virtual void UpdateUI() {
			ImGui::Begin(WindowName.c_str());
			
			if (ImGui::MenuItem(valueName.c_str())) { 
			
				value = !value;

				if (value && onPress != nullptr)
					onPress();

			}

			ImGui::End();
		}

		virtual void Reset() {  }
		virtual void OnContextChanged(const std::shared_ptr<Object>&) {  }

	private:
		bool value;
		std::function<void()> onPress;
		std::string valueName;
	};
}
