#include <iostream>
#include <fstream>
#include <glad/include/glad/glad.h>
#include <GLFW/glfw3.h>
#include "src/Shader.hpp"
/**
 * EXCERCISE 4
 *  Create a vertex shader that draws the triangle upside-down
 * EXCERCISE 5
 *  Create a uniform to displace the vertices' position towards right
 * EXCERCISE 6
 *  Use the "out" keyword to output the vertices' position to the fragment shader & assign those to the color
 * 
 * TO COMPILE:
 *      g++ excercise_3_5_6.cpp ./../include/glad/include/glad.c ./../include/src/Shader.cpp -Wall -Wextra -lglfw3 -ldl -lX11 -pthread -I./../include/ -o excercise_3_5_6
*/

const std::string SRC_VERTEX =
    "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "out vec3 vertexPos;\n"
    "uniform vec3 displacementOffset;\n"
    "void main() {\n"
    // send output to fragment shader and displace it
    "   vertexPos = displacementOffset + aPos.xyz;\n"
    "   vertexPos.y = -vertexPos.y;\n" // upside-down 
    "   gl_Position = vec4(vertexPos, 1.0);\n"
    "}\n";
const std::string SRC_FRAGMENT =
    "#version 330 core\n"
    "in vec3 vertexPos;\n" // receive from vertex shader
    "out vec4 FragColor;\n"
    "void main() {\n"
    "   FragColor = vec4(vertexPos.xyz, 1.0);\n"
    "}\n";

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
    uint triangle1VAO, triangle2VAO;
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
    Shader shader(SRC_VERTEX, SRC_FRAGMENT);
    triangle1VAO = CreateTriangle(triangle1Vertices);
    triangle2VAO = CreateTriangle(triangle2Vertices);

    shader.Use();
    shader.SetVec3("displacementOffset", 0.25f, 0, 0);

    // Main loop
    while (!glfwWindowShouldClose(window)) {

        HandleInput(window);

        // rendering
        glClearColor(0.2f, 0.3f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glBindVertexArray(triangle1VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glBindVertexArray(triangle2VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwTerminate();
    return 0;
}