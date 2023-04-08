#include "window_context.hpp"

std::function<void(const int&, const int&)> WindowContext::onResize = nullptr;

void WindowContext::OnWindowResized(const int& w, const int& h) {
	
	if (onResize == nullptr)
		return;

	onResize(w, h);
}

void WindowContext::OnResize(std::function<void(const int&, const int&)> func) {
	onResize = func;
}

WindowContext::WindowContext() {

}
