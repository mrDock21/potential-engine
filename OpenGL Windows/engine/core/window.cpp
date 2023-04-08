#include "window.hpp"

// global callbacks
void OnResize(GLFWwindow* window, int w, int h) {
    glViewport(0, 0, w, h);
    WindowContext::OnWindowResized(w, h);
}

void OnGlfwError(int id, const char* error) {
    std::cout << "[Glfw::ERROR::" << id << "] " << error << std::endl;
}

Window::Window(const std::string& wndName, const int& width, const int& height) {
    
    this->wndRenderBuffer = GL_COLOR_BUFFER_BIT;
    this->width = width;
    this->height = height;
    
    // GLFW internal
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
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
    // send main thread to window (must be called before GLAD and ImGUI!!! :O)
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync

    // init IMGUI
    InitImGui();
    
    InitGlad();

    Input::Focus(window);
    // couldn't think of something better...
    WindowContext::OnResize([this](const int& w, const int& h) {
        this->Resize(w, h);
    });
}

void Window::InitGlad() const {

    glfwSetWindowSizeCallback(window, OnResize);
    //glfwSetFramebufferSizeCallback(window, OnFrameResize);
    
    std::cout << "Window created! :D" << std::endl;

    // init glad
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        throw "[LOG] GLAD loader initialization failed!";

    glViewport(0, 0, width, height);
}

void Window::InitImGui() {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    imgui_io = &ImGui::GetIO();
    imgui_io->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;   // Enable Keyboard Controls
    imgui_io->ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;    // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();
    
    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 130");


    // Load Fonts
    // - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
    // - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
    // - If the file cannot be loaded, the function will return NULL. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
    // - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
    // - Use '#define IMGUI_ENABLE_FREETYPE' in your imconfig file to use Freetype for higher quality font rendering.
    // - Read 'docs/FONTS.md' for more instructions and details.
    // - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
    // - Our Emscripten build process allows embedding fonts to be accessible at runtime from the "fonts/" folder. See Makefile.emscripten for details.
    //io.Fonts->AddFontDefault();
    //io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\segoeui.ttf", 18.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
    //ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesJapanese());
    //IM_ASSERT(font != NULL);

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

        // UI stuff per frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        HandleUI();

        HandleInput();

        // rendering
        ImGui::Render();
        glClearColor(0.2f, 0.3f, 0.2f, 1.0f);
        glClear(wndRenderBuffer);
        
        OnRender();

        Time::Update();

        ImGui::EndFrame();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Cleanup ImGui
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwTerminate();
}

void Window::Resize(const int& w, const int& h) {
    width = w;
    height = h;
}

/**
 * Called in MainLoop to check user's input
*/
void Window::HandleInput() {
    if (Input::PressedESC())
        Close();
}

void Window::HandleUI() {
    // defined by user (if needed)
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
 * @returns This window's framereate
*/
float Window::Framerate() const {
    return imgui_io->Framerate;
}

/**
 * Returns the point at the center of the screen
*/
glm::vec2 Window::GetCenter() const {
    return glm::vec2(width / 2, height / 2);
}