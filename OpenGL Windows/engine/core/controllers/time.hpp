#pragma once

#include <GLFW/glfw3.h>

class Time {
public:
	/*	Current delta time */
	static float deltaTime;

	static void Update() {
		float currTime = (float)glfwGetTime();

		deltaTime = currTime - prevTime;
		prevTime = currTime;
	}

private:
	Time();
	static float prevTime;
};