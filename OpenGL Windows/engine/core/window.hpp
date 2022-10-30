#pragma once

#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shading/Shader.hpp"
#include "shading/Texture.hpp"
#include "geometry/Mesh.hpp"
#include "controllers/input.hpp"
#include "types/types.hpp"

// callbacks
void OnResize(GLFWwindow*, int, int);
void OnGlfwError(int, const char*);

/**
 * Handles Glfw initialization and input handling
 * [NOTE] Do not use this directly, inheritate from this instead
*/
class Window {
    public:
        enum class WndBuffer{ Color, Depth };

        Window(const std::string&, const int&, const int&);
        void MainLoop();
        void Close() const;

        void Enable(const WndBuffer&);
        void Disable(const WndBuffer&);
        float GetAspectRatio() const;
        glm::vec2 GetCenter() const;

        // Called by window just before enetring the Main loop
        virtual void OnMainLoopInit() = 0;
        // Called by the window each frame when rendering
        virtual void OnRender() = 0;
        // Can be customized (and the object may change...)
        virtual void HandleInput();

    private:
        GLFWwindow* window;
        uint wndRenderBuffer;
        int width, height;

        void InitGlad() const;
};