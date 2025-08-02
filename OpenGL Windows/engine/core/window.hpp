#pragma once

#include <iostream>
#include <memory>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "imgui/imgui.h"
#include "imgui/backend/imgui_impl_glfw.h"
#include "imgui/backend/imgui_impl_opengl3.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shading/Shader.hpp"
#include "shading/Texture.hpp"
#include "geometry/Mesh.hpp"
#include "controllers/input.hpp"
#include "controllers/time.hpp"
#include "types/types.hpp"
#include "frame_buffers/frame_buffer_quad.hpp"

#include "controllers/window_context.hpp"

// global callbacks for GLFW
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

        void Resize(const int&, const int&);

        void Enable(const WndBuffer&);
        void Disable(const WndBuffer&);

        // Utils
        float GetAspectRatio() const;
        float Framerate() const;
        glm::vec2 GetCenter() const;

        // Called by window just before entering the Main loop
        virtual void OnMainLoopInit() = 0;
        // Called by the window each frame, when rendering
        virtual void OnRender() = 0;
        // Can be customized (and the object may change...)
        virtual void HandleInput();
        // Can be customized (UI may change...)
        virtual void HandleUI();

    private:
        GLFWwindow* window;
        ImGuiIO* imgui_io;
        CEngine::uint wndRenderBuffer;
        int width, height;

        void InitGlad() const;
        void InitImGui();
};