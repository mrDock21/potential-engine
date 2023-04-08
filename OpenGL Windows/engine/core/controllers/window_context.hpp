#pragma once

#include <functional>

class WindowContext {
public:
	static void OnWindowResized(const int&, const int&);
	static void OnResize(std::function<void(const int&, const int&)> func);
private:
	// store a lambda
	static std::function<void(const int&, const int&)> onResize;
	WindowContext();
};

