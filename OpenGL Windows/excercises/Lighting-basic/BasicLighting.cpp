#include <iostream>
#include <vector>
#include <fstream>

#include "src/window.hpp"
#include "src/camera.hpp"

/**
 * TO COMPILE:
 *      g++ BasicLighting.cpp ./../../include/glad/include/glad.c ./../../include/src/window.cpp ./../../include/src/Shader.cpp ./../../include/src/Texture.cpp ./../../include/src/Mesh.cpp ./../../include/src/element_buffer.cpp ./../../include/src/vertex_buffer.cpp ./../../include/src/camera.cpp ./../../include/src/input.cpp -Wall -Wextra -lglfw3 -ldl -lX11 -pthread -I./../../include/ -o BasicLighting
 *
 * WITHOUT EXTRA WARNINGS:
 *      g++ BasicLighting.cpp ./../../include/glad/include/glad.c ./../../include/src/window.cpp ./../../include/src/Shader.cpp ./../../include/src/Texture.cpp ./../../include/src/Mesh.cpp ./../../include/src/element_buffer.cpp ./../../include/src/vertex_buffer.cpp ./../../include/src/camera.cpp ./../../include/src/input.cpp -Wall -lglfw3 -ldl -lX11 -pthread -I./../../include/ -o BasicLighting
*/

const std::string SRC_VERTEX =
    "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "layout (location = 1) in vec2 aTexCoord;\n"
    "out vec2 texCoord;\n"
    "uniform mat4 model;\n"
    "uniform mat4 view;\n"
    "uniform mat4 projection;\n"
    "void main() {\n"
    // send output to fragment shader and displace it
    "   texCoord = aTexCoord;\n"
    "   gl_Position = projection * view * model * vec4(aPos.xyz, 1.0);\n"
    "}\n";
const std::string SRC_FRAGMENT =
    "#version 330 core\n"
    "in vec2 texCoord;\n"
    "out vec4 FragColor;\n"
    "uniform vec3 objectColor;\n"
    "uniform vec3 lightColor;\n"
    "uniform sampler2D ourTexture;\n"
    "void main() {\n"
    "   FragColor = texture(ourTexture, texCoord) * vec4(lightColor * objectColor, 1.0);\n"
    "}\n";

const std::string SRC_FRAGMENT_LIGHT =
    "#version 330 core\n"
    "out vec4 FragColor;\n"
    "uniform vec3 lightColor;\n"
    "void main() {\n"
    "   FragColor = vec4(lightColor, 1.0);\n"
    "}\n";

class CameraMove : public Window {
    public:
        CameraMove(const std::string& wnd, const int& w, const int h) 
            : Window(wnd, w, h), mainCamera(), 
              deltaTime(0), prevTime(0) { 

                cubesPositions.push_back(glm::vec3(0, 0, 0));
                cubesPositions.push_back(glm::vec3(2, 5, -15));
                cubesPositions.push_back(glm::vec3(-1.5f, -2.2f, -2.5f));
                cubesPositions.push_back(glm::vec3(-3.8f, -2.0f, -12.3f));
                cubesPositions.push_back(glm::vec3(2.4f, -0.4f, -3.5f));
                cubesPositions.push_back(glm::vec3(-1.7f, 3.0f, -7.5f));
                cubesPositions.push_back(glm::vec3(1.3f, -2.0f, -2.5f));
                cubesPositions.push_back(glm::vec3(1.5f, 2.0f, -2.5f));
                cubesPositions.push_back(glm::vec3(1.5f, 0.2f, -1.5f));
                cubesPositions.push_back(glm::vec3(-1.3f, 1.0f, -1.5f));

                //Input::LockCursor();
            }

        ~CameraMove() { 
            std::cout << "[Multiple-Cubes::FreesMemory]" << std::endl;
            
            for (int i(0); i < NUM_CUBES; i++) {
                delete cubes.at(i);
            }
            delete lightSource;
            cubes.clear();
            cubesPositions.clear();
        }

        void CreateLight(float* vertices, const u_long& vsize, uint* indices, const u_long& isize) {
            Shader s(SRC_VERTEX, SRC_FRAGMENT_LIGHT);

            lightSource = new Mesh(vertices, vsize, indices, isize);
            lightSource->Use();
            // position attribute
            lightSource->SetAttribute(0, 3, GL_FLOAT, false, 5 * sizeof(float), 0);
            // uv's attribute
            lightSource->SetAttribute(1, 2, GL_FLOAT, false, 5 * sizeof(float), 3 * sizeof(float));
            s.Use();
            lightSource->SetShader(s);
        }
        
        void UpdateMouseLook() {
            // jaw
            if (Input::PressedLEFT())
                mainCamera.Rotate(glm::vec3(0, -360 * deltaTime * sensitivity, 0));
            if (Input::PressedRIGHT())
                mainCamera.Rotate(glm::vec3(0, 360 * deltaTime * sensitivity, 0));
            // pitch
            if (Input::PressedUP())
                mainCamera.Rotate(glm::vec3(360 * deltaTime * sensitivity, 0, 0));
            if (Input::PressedDOWN())
                mainCamera.Rotate(glm::vec3(-360 * deltaTime * sensitivity, 0, 0));
        }

        void HandleInput() {
            if (Input::PressedW())
                // move forward (since camera faces towards negative z)
                mainCamera.Move(mainCamera.GetForward() * deltaTime);
            if (Input::PressedS())
                // move backwards (camera faces towards negative z)
                mainCamera.Move(-mainCamera.GetForward() * deltaTime);
            if (Input::PressedA())
                // move left
                mainCamera.Move(-mainCamera.GetRight() * deltaTime);
            if (Input::PressedD())
                // move right
                mainCamera.Move(mainCamera.GetRight() * deltaTime);

            if (Input::PressedESC())
                Close();

            UpdateMouseLook();
        }

        /**
         * Excercise is to move the camera with keyboard
        */
        void OnMainLoopInit() {
            float rect[] = {
                // vertices       // uv's
                // front face
                0.5f, 0.5f, 0,    1.0f, 1.0f,  // top-right
                -0.5f, 0.5f, 0,   0.0f, 1.0f,  // top-left
                0.5f, -0.5f, 0,   1.0f, 0.0f,  // bottom-right
                -0.5f, -0.5f, 0,   0.0f, 0.0f,   // bottom-left
                // back face
                0.5f, 0.5f, -1,    1.0f, 1.0f,  // top-right
                -0.5f, 0.5f, -1,   0.0f, 1.0f,  // top-left
                0.5f, -0.5f, -1,   1.0f, 0.0f,  // bottom-right
                -0.5f, -0.5f, -1,   0.0f, 0.0f   // bottom-left
            };
            uint indices[] = {
                // front face
                0, 1, 2,    // triangle 1
                3, 1, 2,     // triangle 2
                // back face
                4, 5, 6,
                7, 5, 6,
                // right face
                0, 4, 6,
                0, 2, 6,
                // left face
                1, 5, 7,
                7, 3, 1,
                // upper face
                0, 1, 5,
                4, 5, 0,
                // bottom face
                2, 3, 7,
                6, 7, 2
            };
            Mesh* mesh(nullptr);
            Texture mainTex("./../../assets/oldman_meme.jpg", Texture::TexFormat::RGB, Texture::PixelFormat::RGB);
            Shader shader(SRC_VERTEX, SRC_FRAGMENT), 
                   shaderLight(SRC_VERTEX, SRC_FRAGMENT_LIGHT);
            // enable ZTest buffering!
            Enable(WndBuffer::Depth);

            for (int i(0); i < NUM_CUBES; i++) {
                mesh = new Mesh(rect, sizeof(rect), indices, sizeof(indices));

                // position attribute
                mesh->SetAttribute(0, 3, GL_FLOAT, false, 5 * sizeof(float), 0);
                // uv's attribute
                mesh->SetAttribute(1, 2, GL_FLOAT, false, 5 * sizeof(float), 3 * sizeof(float));
                
                mesh->Use();
                // to bind with current VAO
                mainTex.Use();
                shader.Use();
                glActiveTexture(GL_TEXTURE0);
                shader.SetInt("ourTexture", 0);
                
                // put here to make sure VAO's been binded
                mesh->SetTexture(mainTex);
                mesh->SetShader(shader);

                cubes.push_back(mesh);
            }

            CreateLight(rect, sizeof(rect), indices, sizeof(indices));
        }

        void OnRenderLight(const glm::mat4& view, const glm::mat4& proj) {
            glm::mat4 model(1.0f);

            model = glm::translate(model, glm::vec3(1.0f, 1.0f, 1.0f));
            model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));

            lightSource->Render();
            lightSource->GetShader().SetMat4x4("model", model);
            lightSource->GetShader().SetMat4x4("view", view);
            lightSource->GetShader().SetMat4x4("projection", proj);
            lightSource->GetShader().SetVec3("lightColor", LIGHT_COLOR);
            glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        }

        void OnRender() {
            Mesh *mesh(nullptr);
            // transformations
            glm::mat4 model;
            glm::mat4 view = mainCamera.GetViewMatrix();
            float angle; 
            glm::mat4 proj = 
                glm::perspective(45.0, (double)GetAspectRatio(), 0.1, 100.0);
            // since all of 'em use the same pointer
            cubes.at(0)->GetShader().SetMat4x4("view", view);
            cubes.at(0)->GetShader().SetMat4x4("projection", proj);

            for (int i(0); i < NUM_CUBES; i++) {
                mesh = cubes.at(i);
                // Excercise: each third cube rotates by time
                angle = (i + 1) % 3 == 0 ? (float)glfwGetTime() : glm::radians(20.0f * i);
                model = glm::translate(glm::mat4(1.0f), cubesPositions.at(i));
                model = glm::rotate(model, angle, glm::vec3(1, 0.3f, 0.5f));

                mesh->Render();
                // update uniforms
                mesh->GetShader().SetMat4x4("model", model);
                mesh->GetShader().SetVec3("objectColor", OBJECT_COLOR);
                mesh->GetShader().SetVec3("lightColor", LIGHT_COLOR);
                glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
            }

            OnRenderLight(view, proj);

            // update delta time
            float currTime = (float)glfwGetTime();
            deltaTime = currTime - prevTime;
            prevTime = currTime;
        }
    
    private:
        Camera mainCamera;
        std::vector<Mesh*> cubes;
        std::vector<glm::vec3> cubesPositions;
        Mesh* lightSource;
        float deltaTime, prevTime;

        const int NUM_CUBES = 10;
        const float cameraSpeed = 0.1f, sensitivity = 0.1f;
        const glm::vec3 LIGHT_COLOR = glm::vec3(1.0f, 1.0f, 1.0f),
                        OBJECT_COLOR = glm::vec3(1.0f, 0.5f, 0.31f);
};

int main() {
    CameraMove wnd("LearnOpenGL", 800, 600);
    wnd.MainLoop();
    return 0;
}