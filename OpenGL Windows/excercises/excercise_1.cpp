#include <iostream>
#include <fstream>
#include <glad/include/glad/glad.h>
#include <GLFW/glfw3.h>

/**
 * EXCERCISE 1
 * Draw 2 triangles next to each other by adding more vertices
 * 
 * TO COMPILE:
 *      g++ excercise_1.cpp ./../include/glad/include/glad.c -lglfw3 -ldl -lX11 -pthread -I./../include/ -o excercise_1
*/

void glfw_window_resize_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void glfw_error(int id, const char* error) {
    std::cout << "[ERROR:" << id << "] " << error << std::endl;
}

GLFWwindow* InitGLFWWindow() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    // init glfw window
    GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
    if (window == NULL){
        std::cout << "Couldn't open window :(" << std::endl;
        glfwTerminate();
        return NULL;
    }
    // send main thread to window (must be called before GLAD!!! :O)
    glfwMakeContextCurrent(window);
    return window;
}

GLFWwindow* Init(){ 
    // when user resizes callback
    glfwSetErrorCallback(&glfw_error);

    GLFWwindow* window;
    
    if ((window = InitGLFWWindow()) == NULL) {
        std::cout << "[LOG] GLFW window initialization failed!" << std::endl;
        return nullptr;
    }
    glfwSetWindowSizeCallback(window, glfw_window_resize_callback);
    
    std::cout << "Window created! :D" << std::endl;

    // init glad
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        std::cout << "[LOG] GLAD loader initialization failed!" << std::endl;
        return nullptr;
    }
    glViewport(0, 0, 800, 600);

    return window;
}

void HandleInput(GLFWwindow* wnd) {
    if (glfwGetKey(wnd, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(wnd, true);
}

std::string ReadFile(const char* filename) {
    std::ifstream stream = std::ifstream(filename);
    std::string str, aux;
    if (!stream.is_open()) {
        std::cout << "[READ-FILE:ERROR] Couldn't open " << filename << std::endl;
        stream.close();
        return "";
    }
    stream.seekg(0, std::ios::end);
    str.resize(stream.tellg());
    stream.seekg(0, std::ios::beg);
    stream.read(&str[0], str.size());
    stream.close();
    return std::string(str);
}

uint CompileShader(const char* shader, uint shaderType) {
    std::string source = ReadFile(shader);
    std::cout << "Source => " << source << std::endl;
    const char* s = source.c_str();
    uint id = glCreateShader(shaderType);
    int success;
    char infolog[512];

    glShaderSource(id, 1, &s, NULL);
    glCompileShader(id);

    glGetShaderiv(id, GL_COMPILE_STATUS, &success);

    if (!success) {
        glGetShaderInfoLog(id, sizeof(infolog), NULL, infolog);
        std::cout << "[SHADER-COMPILE-ERROR] " << shader << " => " << infolog << std::endl;
        return 0;
    }
    return id;
}

void BuildShaders() {
    uint vertexShader = CompileShader("./../shaders/vertex_shader.shader", GL_VERTEX_SHADER),
        fragmentShader = CompileShader("./../shaders/fragment_shader.shader", GL_FRAGMENT_SHADER);
    uint program = glCreateProgram();
    char infolog[512];
    int success;

    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);

    glGetShaderiv(program, GL_LINK_STATUS, &success);

    if (success) {
        std::cout << "[SHADER-LOG] Shader linking successful" << std::endl;
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        glUseProgram(program);
    }
    else {
        glGetShaderInfoLog(program, sizeof(infolog), NULL, infolog);
        std::cout << "[SHADER-COMPILE-ERROR] " << infolog << std::endl;
    }
}

int main() {
    GLFWwindow* window = Init();
    float vertices[] = {
        0.5f, 0.5f, 0,
        0.25f, -0.5f, 0,
        0.75f, -0.5f, 0,

        -0.5f, 0.5f, 0,
        -0.25f, -0.5f, 0,
        -0.75f, -0.5f, 0
    };
    uint indices[] = {
        0, 1, 2,    // first triangle
        3, 4, 5     // second triangle
    };
    uint VBO, EBO, VAO;
    // Vertex array object
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    // vertex buffer object -> Gets binded to current VAO
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // element array object -> Gets binded to current VAO
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, false, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    BuildShaders();
    glBindVertexArray(VAO);
    // it needs to have indices binded in order to draw
    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO); but VAO also takes care of it!
    // Main loop
    while (!glfwWindowShouldClose(window)) {

        HandleInput(window);

        // rendering
        glClearColor(0.2f, 0.3f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwTerminate();
    return 0;
}