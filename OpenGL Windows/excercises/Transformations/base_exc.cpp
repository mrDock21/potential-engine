#include <iostream>
#include <fstream>

#include <glad/include/glad/glad.h>
#include <GLFW/glfw3.h>

#include <vendor/glm/glm.hpp>
#include <vendor/glm/gtc/matrix_transform.hpp>
#include <vendor/glm/gtc/type_ptr.hpp>

#include "src/Shader.hpp"
#include "src/Texture.hpp"

/**
 * TO COMPILE:
 *      g++ base_exc.cpp ./../../include/glad/include/glad.c ./../../include/src/Shader.cpp ./../../include/src/Texture.cpp -Wall -Wextra -lglfw3 -ldl -lX11 -pthread -I./../../include/ -o base_exc
*/
const std::string SRC_VERTEX =
    "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "layout (location = 1) in vec3 aColor;\n"
    "layout (location = 2) in vec2 aTexCoord;\n"
    "out vec3 ourColor;\n"
    "out vec2 texCoord;\n"
    "uniform mat4 transformation;\n"
    "void main() {\n"
    // send output to fragment shader and displace it
    "   ourColor = aColor;"
    "   texCoord = aTexCoord;"
    "   gl_Position = transformation * vec4(aPos.xyz, 1.0);\n"
    "}\n";
const std::string SRC_FRAGMENT =
    "#version 330 core\n"
    "in vec3 ourColor;\n" // receive from vertex shader
    "in vec2 texCoord;"
    "out vec4 FragColor;\n"
    "uniform sampler2D ourTexture;"
    "void main() {\n"
    "   FragColor = texture(ourTexture, texCoord);\n"
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

uint CreateRectangle(float vertices[], u_long vertsSize, uint indices[], u_long indicesSize) {
    uint VAO, VBO, EBO;
    // vertex array object
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    // vertex buffer
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertsSize, vertices, GL_STATIC_DRAW);
    // elements (index) buffer
    // element array object -> Gets binded to current VAO
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesSize, indices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, false, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // vertex colors attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, false, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // uv's attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, false, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
    return VAO;
}

int main() {
    GLFWwindow* window = Init();
    uint VAOrect1, VAOrect2;
    float rect[] = {
        // vertices        // vertex colors    // uv's
        0.5f, 0.5f, 0,    1.0f, 0.0f, 0.0f,   1.0f, 1.0f,  // top-right
        -0.5f, 0.5f, 0,   0.0f, 1.0f, 0.0f,   0.0f, 1.0f,  // top-left
        0.5f, -0.5f, 0,   0.0f, 0.0f, 1.0f,   1.0f, 0.0f,  // bottom-right
        -0.5f, -0.5f, 0,   1.0f, 1.0f, 0.0f,   0.0f, 0.0f   // bottom-left
    };
    uint indices[] = {
        0, 1, 2,    // triangle 1
        3, 1, 2     // triangle 2
    };
    Shader shader1(SRC_VERTEX, SRC_FRAGMENT);
    
    VAOrect1 = CreateRectangle(rect, sizeof(rect), indices, sizeof(indices));

    glBindVertexArray(VAOrect1);
    // put here to make sure VAO's been binded
    Texture texture("./../../assets/meme.jpg", Texture::TexFormat::RGB, Texture::PixelFormat::RGB);

    glBindVertexArray(VAOrect1);
    glActiveTexture(GL_TEXTURE0);
    texture.Use();
    shader1.SetInt("ourTexture", 0);

    Shader shader2(SRC_VERTEX, SRC_FRAGMENT);
    
    VAOrect2 = CreateRectangle(rect, sizeof(rect), indices, sizeof(indices));

    glBindVertexArray(VAOrect2);
    // put here to make sure VAO's been binded
    Texture texture2("./../../assets/oldman_meme.jpg", Texture::TexFormat::RGB, Texture::PixelFormat::RGB);

    glBindVertexArray(VAOrect2);
    glActiveTexture(GL_TEXTURE0);
    texture2.Use();
    shader2.SetInt("ourTexture", 0);

    // TODO
    // Main loop
    while (!glfwWindowShouldClose(window)) {

        HandleInput(window);

        // rendering
        glClearColor(0.2f, 0.3f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // rect 1
        glBindVertexArray(VAOrect1);
        // transformations
        glm::mat4 trans = glm::mat4(1.0f);
        // we first scale by half, and then rotate
        trans = glm::translate(trans, glm::vec3(0.5, -0.5, 0));
        trans = glm::rotate(trans, (float)glfwGetTime(), glm::vec3(0.0, 0.0, 1.0));
        shader1.SetMat4x4("transformation", trans);
        texture.Use();
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        // rect 2
        glBindVertexArray(VAOrect2);
        // transformations
        float scale = glm::sin((float)glfwGetTime());
        trans = glm::mat4(1.0f);
        // we first scale by half, and then rotate
        trans = glm::translate(trans, glm::vec3(-0.5, 0.5, 0));
        trans = glm::scale(trans, glm::vec3(scale, scale, scale));
        shader2.SetMat4x4("transformation", trans);
        texture2.Use();
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwTerminate();
    return 0;
}