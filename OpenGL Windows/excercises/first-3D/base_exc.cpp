#include <iostream>
#include <fstream>

#include "src/window.hpp"

/**
 * TO COMPILE:
 *      g++ base_exc.cpp ./../../include/glad/include/glad.c ./../../include/src/window.cpp ./../../include/src/Shader.cpp ./../../include/src/Texture.cpp ./../../include/src/Mesh.cpp ./../../include/src/element_buffer.cpp ./../../include/src/vertex_buffer.cpp -Wall -Wextra -lglfw3 -ldl -lX11 -pthread -I./../../include/ -o base_exc
*/

const std::string SRC_VERTEX =
    "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "layout (location = 1) in vec3 aColor;\n"
    "layout (location = 2) in vec2 aTexCoord;\n"
    "out vec3 ourColor;\n"
    "out vec2 texCoord;\n"
    "uniform mat4 model;\n"
    "uniform mat4 view;\n"
    "uniform mat4 projection;\n"
    "void main() {\n"
    // send output to fragment shader and displace it
    "   ourColor = aColor;"
    "   texCoord = aTexCoord;"
    "   gl_Position = projection * view * model * vec4(aPos.xyz, 1.0);\n"
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

class Simple3D : public Window {
    public:
        Simple3D(const std::string& wnd, const int& w, const int h) 
            : Window(wnd, w, h), mesh(nullptr), shader(nullptr) { }

        ~Simple3D() { 
            std::cout << "[Simple3D::FreesMemory]" << std::endl;
            delete mesh; 
            delete shader; 
        }
        
        /**
         * Excercise is to simple show 3D
        */
        void OnMainLoopInit() {
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
            shader = new Shader(SRC_VERTEX, SRC_FRAGMENT);
            mesh = new Mesh(rect, sizeof(rect), indices, sizeof(indices));

            // position attribute
            mesh->SetAttribute(0, 3, GL_FLOAT, false, 8 * sizeof(float), 0);
            // vertex colors attribute
            mesh->SetAttribute(1, 3, GL_FLOAT, false, 8 * sizeof(float), 3 * sizeof(float));
            // uv's attribute
            mesh->SetAttribute(2, 2, GL_FLOAT, false, 8 * sizeof(float), 6 * sizeof(float));
            
            mesh->Use();
            // put here to make sure VAO's been binded
            mesh->SetTexture(
                Texture("./../../assets/oldman_meme.jpg", Texture::TexFormat::RGB, Texture::PixelFormat::RGB)
            );
            glActiveTexture(GL_TEXTURE0);
            mesh->GetTexture().Use();
            shader->SetInt("ourTexture", 0);
            mesh->SetShader(*shader);
        }

        void OnRender() {
            // transformations
            glm::mat4 model = 
                glm::rotate(glm::mat4(1.0), (float)glfwGetTime(), glm::vec3(0, 1, 0));
            glm::mat4 view = 
                glm::translate(glm::mat4(1.0), glm::vec3(0, 0, -3));
            glm::mat4 proj = 
                glm::perspective(45.0, (double)GetAspectRatio(), 0.1, 100.0);
            
            mesh->Render();
            // update uniforms
            mesh->GetShader().SetMat4x4("model", model);
            mesh->GetShader().SetMat4x4("view", view);
            mesh->GetShader().SetMat4x4("projection", proj);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        }
    
    private:
        Mesh *mesh;
        Shader *shader;
};

int main() {
    Simple3D wnd("LearnOpenGL", 800, 600);
    wnd.MainLoop();
    return 0;
}