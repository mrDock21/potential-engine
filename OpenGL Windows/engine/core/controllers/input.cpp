#include "input.hpp"

GLFWwindow* Input::wndFocussed = nullptr;
glm::vec2 Input::mousePos = glm::vec2(0, 0);

Input::Input() { }

/**
 * Checks user's input against given window
 * @param wnd window to be focussed
*/
void Input::Focus(GLFWwindow* wnd) {
    Input::wndFocussed = wnd;
}

/**
 * Will hide the mouse
*/
void Input::LockCursor() {
    glfwSetInputMode(Input::wndFocussed, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

/**
 * Returns true if W key is pressed during this frame
*/
bool Input::PressedW() {
    if (glfwGetKey(Input::wndFocussed, GLFW_KEY_W) == GLFW_PRESS)
        return true;
    return false;
}

/**
 * Returns true if A key is pressed during this frame
*/
bool Input::PressedA() {
    if (glfwGetKey(Input::wndFocussed, GLFW_KEY_A) == GLFW_PRESS)
        return true;
    return false;
}

/**
 * Returns true if S key is pressed during this frame
*/
bool Input::PressedS() {
    if (glfwGetKey(Input::wndFocussed, GLFW_KEY_S) == GLFW_PRESS)
        return true;
    return false;
}

/**
 * Returns true if D key is pressed during this frame
*/
bool Input::PressedD() {
    if (glfwGetKey(Input::wndFocussed, GLFW_KEY_D) == GLFW_PRESS)
        return true;
    return false;
}

/**
 * Returns true if Q key is pressed during this frame
*/
bool Input::PressedQ() {
    if (glfwGetKey(Input::wndFocussed, GLFW_KEY_Q) == GLFW_PRESS)
        return true;
    return false;
}

/**
 * Returns true if E key is pressed during this frame
*/
bool Input::PressedE() {
    if (glfwGetKey(Input::wndFocussed, GLFW_KEY_E) == GLFW_PRESS)
        return true;
    return false;
}

bool Input::PressedUP() {
    if (glfwGetKey(Input::wndFocussed, GLFW_KEY_UP) == GLFW_PRESS)
        return true;
    return false;
}

bool Input::PressedLEFT() {
    if (glfwGetKey(Input::wndFocussed, GLFW_KEY_LEFT) == GLFW_PRESS)
        return true;
    return false;
}

bool Input::PressedDOWN() {
    if (glfwGetKey(Input::wndFocussed, GLFW_KEY_DOWN) == GLFW_PRESS)
        return true;
    return false;
}

bool Input::PressedRIGHT() {
    if (glfwGetKey(Input::wndFocussed, GLFW_KEY_RIGHT) == GLFW_PRESS)
        return true;
    return false;
}

/**
 * Returns true if ESC key is pressed during this frame
*/
bool Input::PressedESC() {
    if (glfwGetKey(Input::wndFocussed, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        return true;
    return false;
}

/**
 * Returns current mouse position
*/
glm::vec2 Input::GetMouse() {
    double x, y;

    glfwGetCursorPos(Input::wndFocussed, &x, &y);
    Input::mousePos.x = (float)x;
    Input::mousePos.y = (float)y;

    return Input::mousePos;
}