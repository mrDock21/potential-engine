#include "window.hpp"

// global callbacks
void OnResize(GLFWwindow* window, int w, int h) {
    glViewport(0, 0, w, h);
}

void OnGlfwError(int id, const char* error) {
    std::cout << "[Glfw::ERROR::" << id << "] " << error << std::endl;
}

Window::Window(const std::string& wndName, const int& width, const int& height) {
    
    this->wndRenderBuffer = GL_COLOR_BUFFER_BIT;
    this->width = width;
    this->height = height;
    
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    // when error callback
    glfwSetErrorCallback(&OnGlfwError);
    // init glfw window
    window = glfwCreateWindow(width, height, wndName.c_str(), NULL, NULL);
    
    if (window == nullptr) {
        glfwTerminate();
        // change this later
        throw "[WINDOW-ERROR::Couldn't create window!";
    }
    // send main thread to window (must be called before GLAD!!! :O)
    glfwMakeContextCurrent(window);

    InitGlad();

    Input::Focus(window);
}

void Window::InitGlad() const {

    glfwSetWindowSizeCallback(window, OnResize);
    
    std::cout << "Window created! :D" << std::endl;

    // init glad
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        throw "[LOG] GLAD loader initialization failed!";

    glViewport(0, 0, width, height);
}

/**
 * Will close the window
*/
void Window::Close() const { glfwSetWindowShouldClose(window, true); }

/**
 * Starts rendering main loop
*/
void Window::MainLoop() {
    OnMainLoopInit();

    // Main loop
    while (!glfwWindowShouldClose(window)) {

        HandleInput();

        // rendering
        glClearColor(0.2f, 0.3f, 0.2f, 1.0f);
        glClear(wndRenderBuffer);
        
        OnRender();

        Time::Update();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwTerminate();
}

/**
 * Called in MainLoop to check user's input
*/
void Window::HandleInput() {
    if (Input::PressedESC())
        Close();
}

void Window::Enable(const WndBuffer& type) {
    switch (type)
    {
    case WndBuffer::Color: 
        // color is enabled by default
        break;
    case WndBuffer::Depth:
        //  enable Z-test buffer in rendering 
        glEnable(GL_DEPTH_TEST);
        // update clear buffer bit...
        wndRenderBuffer |= GL_DEPTH_BUFFER_BIT;
        break;
    default:
        std::cout << "[WINDOW::Enable] Requested bit enable's still not implemmented...";
        std::cout << std::endl;
        break;
    }
}

void Window::Disable(const WndBuffer& type) {
    switch (type)
    {
    case WndBuffer::Color: 
        // color is enabled by default
        std::cout << "[WINDOW::Disable] You don't want to disable color son!";
        std::cout << std::endl;
        break;
    case WndBuffer::Depth:
        //  enable Z-test buffer in rendering 
        glDisable(GL_DEPTH_TEST);
        // update clear buffer bit...
        wndRenderBuffer &= !GL_DEPTH_BUFFER_BIT;
        std::cout << "[WINDOW::Disable] Depth diabled!" << std::endl;
        break;
    default:
        std::cout << "[WINDOW::Enable] Requested bit enable's still not implemmented...";
        std::cout << std::endl;
        break;
    }
}

/**
 * @returns This window's aspect ratio (weight/height)
*/
float Window::GetAspectRatio() const {
    return (float)width / (float)height;
}

/**
 * Returns the point at the center of the screen
*/
glm::vec2 Window::GetCenter() const {
    return glm::vec2(width / 2, height / 2);
}