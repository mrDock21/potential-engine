#pragma once

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>

class Input {
    public:
        static void Focus(GLFWwindow*);

        static void LockCursor();

        // input helpers
        static bool PressedW();
        static bool PressedA();
        static bool PressedS();
        static bool PressedD();
        static bool PressedQ();
        static bool PressedE();
        static bool PressedUP();
        static bool PressedLEFT();
        static bool PressedDOWN();
        static bool PressedRIGHT();
        static bool PressedESC(); 
        static glm::vec2 GetMouse();

    private:
        // this makes this class is only static accesable
        Input();

        static GLFWwindow *wndFocussed;
        static glm::vec2 mousePos;
};

