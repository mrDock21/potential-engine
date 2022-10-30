#include <iostream>
#include <fstream>
#include <glad/include/glad/glad.h>
#include <GLFW/glfw3.h>

/**
 * EXCERCISE 2
 * Create 2 triangles but with different VAOs and VBOs
 * 
 * TO COMPILE:
 *      g++ excercise_3.cpp ./../include/glad/include/glad.c -Wall -lglfw3 -ldl -lX11 -pthread -I./../include/ -o excercise_3
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

uint BuildShaders(const char* filepathFrag) {
    uint vertexShader = CompileShader("./../shaders/vertex_shader.shader", GL_VERTEX_SHADER),
        fragmentShader = CompileShader(filepathFrag, GL_FRAGMENT_SHADER);
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
        return program;
    }
    else {
        glGetShaderInfoLog(program, sizeof(infolog), NULL, infolog);
        std::cout << "[SHADER-PROGRAM-ERROR] " << infolog << std::endl;
    }
    return 0;
}

uint CreateTriangle(float vertices[]) {
    uint VAO, VBO;
    // unbind...
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 9, vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, false, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    return VAO;
}

int main() {
    GLFWwindow* window = Init();
    uint triangle1VAO, triangle2VAO,
         shaderOrange, shaderYellow;
    float triangle1Vertices[] = {
        0.5f, 0.5f, 0,
        0.25f, -0.5f, 0,
        0.75f, -0.5f, 0
    };
    float triangle2Vertices[] = {
        -0.5f, 0.5f, 0,
        -0.25f, -0.5f, 0,
        -0.75f, -0.5f, 0
    };
    triangle1VAO = CreateTriangle(triangle1Vertices);
    triangle2VAO = CreateTriangle(triangle2Vertices);

    shaderOrange = BuildShaders("./../shaders/fragment_shader.shader");
    shaderYellow = BuildShaders("./../shaders/frag_shader2.shader");

    // Main loop
    while (!glfwWindowShouldClose(window)) {

        HandleInput(window);

        // rendering
        glClearColor(0.2f, 0.3f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderOrange);
        glBindVertexArray(triangle1VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glUseProgram(shaderYellow);
        glBindVertexArray(triangle2VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwTerminate();
    return 0;
}