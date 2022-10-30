#include <iostream>
#include <vector>
#include <fstream>

#include "src/window.hpp"
#include "src/camera.hpp"

/**
 * TO COMPILE:
 *      g++ BasicPhong.cpp ./../../include/glad/include/glad.c ./../../include/src/window.cpp ./../../include/src/Shader.cpp ./../../include/src/Texture.cpp ./../../include/src/Mesh.cpp ./../../include/src/element_buffer.cpp ./../../include/src/vertex_buffer.cpp ./../../include/src/camera.cpp ./../../include/src/input.cpp -Wall -Wextra -lglfw3 -ldl -lX11 -pthread -I./../../include/ -o BasicPhong
 *
 * WITHOUT EXTRA WARNINGS:
 *      g++ BasicPhong.cpp ./../../include/glad/include/glad.c ./../../include/src/window.cpp ./../../include/src/Shader.cpp ./../../include/src/Texture.cpp ./../../include/src/Mesh.cpp ./../../include/src/element_buffer.cpp ./../../include/src/vertex_buffer.cpp ./../../include/src/camera.cpp ./../../include/src/input.cpp -Wall -lglfw3 -ldl -lX11 -pthread -I./../../include/ -o BasicPhong
*/

const std::string SRC_VERTEX =
    "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "layout (location = 1) in vec3 aNormal;\n"
    "layout (location = 2) in vec2 aTexCoord;\n"
    "out vec2 texCoord;\n"
    "out vec3 normal;\n"
    "out vec4 fragPos;"
    "uniform mat4 model;\n"
    "uniform mat4 view;\n"
    "uniform mat4 projection;\n"
    "void main() {\n"
    // send output to fragment shader and displace it
    "   texCoord = aTexCoord;\n"
    "   normal = (model * vec4(aNormal, 0.0)).xyz;\n"
    "   fragPos = model * vec4(aPos.xyz, 1.0);\n"
    "   gl_Position = projection * view * model * vec4(aPos.xyz, 1.0);\n"
    "}\n";
const std::string SRC_FRAGMENT =
    "#version 330 core\n"
    "in vec2 texCoord;\n"
    "in vec3 normal;\n"
    "in vec4 fragPos;\n"
    "out vec4 FragColor;\n"
    "uniform float ambientFactor;\n"
    "uniform vec3 objectColor;\n"
    "uniform vec3 lightPosition;\n"
    "uniform vec3 lightColor;\n"
    "uniform vec3 cameraPos;\n"
    "uniform sampler2D ourTexture;\n"
    "void main() {\n"
    // diffuse compute
    "   vec3 lightDir = normalize(lightPosition - fragPos.xyz);\n"
    "   float intensity = clamp(dot(lightDir, normal), 0.0, 1.0);\n"
    // specular compute
    "   vec3 viewDir = normalize(cameraPos - fragPos.xyz);\n"
    "   float specular = pow( clamp( dot( reflect(-lightDir, normal), viewDir), 0.0, 1.0), 32);\n"
    // color
    "   vec3 color = lightColor * objectColor * (ambientFactor + intensity);\n"
    "   color += lightColor * specular;\n"
    "   FragColor = texture(ourTexture, texCoord) * vec4(color, 1.0);\n"
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

        void CreateLight(float* vertices, const u_long& vsize, const u_long& esize) {
            Shader s(SRC_VERTEX, SRC_FRAGMENT_LIGHT);

            lightSource = new Mesh(vertices, vsize, esize);
            lightSource->Use();
            // position attribute
            lightSource->SetAttribute(0, 3, GL_FLOAT, false, esize, 0);
            // normals
            lightSource->SetAttribute(1, 3, GL_FLOAT, false, esize, 3 * sizeof(float));
            // uv's attribute
            lightSource->SetAttribute(2, 2, GL_FLOAT, false, esize, 6 * sizeof(float));
                
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
         * Excercise is to create basic phong lighting with
         * ambient light, directional light and specular
        */
        void OnMainLoopInit() {
            float rect[] = {
                // vertices       //normals      // uv's
                // front face
                0.5f, 0.5f, 0,    0, 0, 1.0f,    1.0f, 1.0f,  // triangle
                -0.5f, 0.5f, 0,   0, 0, 1.0f,    0.0f, 1.0f,  
                0.5f, -0.5f, 0,   0, 0, 1.0f,    1.0f, 0.0f,  
                -0.5f, -0.5f, 0,  0, 0, 1.0f,    0.0f, 0.0f,  // triangle
                -0.5, 0.5f, 0,    0, 0, 1.0f,    0.0f, 1.0f,
                0.5f, -0.5f, 0,   0, 0, 1.0f,    1.0f, 0.0f,
                // back face
                0.5f, 0.5f, -1,   0, 0, -1.0f,   1.0f, 1.0f,  // triangle
                -0.5f, 0.5f, -1,  0, 0, -1.0f,   0.0f, 1.0f,  
                0.5f, -0.5f, -1,  0, 0, -1.0f,   1.0f, 0.0f,  
                -0.5f, -0.5f, -1, 0, 0, -1.0f,   0.0f, 0.0f,  // triangle
                -0.5f, 0.5f, -1,  0, 0, -1.0f,   0.0f, 1.0f,
                0.5f, -0.5f, -1,  0, 0, -1.0f,   1.0f, 0.0f,
                // right face
                0.5f, 0.5f, 0,    1.0f, 0, 0,   0.0f, 1.0f,  // triangle
                0.5f, 0.5f, -1,   1.0f, 0, 0,   1.0f, 1.0f,  
                0.5f, -0.5f, 0,   1.0f, 0, 0,   0.0f, 0.0f,  
                0.5f, -0.5f, -1,  1.0f, 0, 0,   1.0f, 0.0f,  // triangle
                0.5f, 0.5f, -1,   1.0f, 0, 0,   1.0f, 1.0f,  
                0.5f, -0.5f, 0,   1.0f, 0, 0,   0.0f, 0.0f,
                // left face
                -0.5f, 0.5f, 0,   -1.0f, 0, 0,   1.0f, 1.0f,  // triangle
                -0.5f, 0.5f, -1,  -1.0f, 0, 0,   0.0f, 1.0f,  
                -0.5f, -0.5f, 0,  -1.0f, 0, 0,   1.0f, 0.0f,  
                -0.5f, -0.5f, -1, -1.0f, 0, 0,   0.0f, 0.0f,  // triangle
                -0.5f, -0.5f, 0,  -1.0f, 0, 0,   1.0f, 0.0f,
                -0.5f, 0.5f, -1,  -1.0f, 0, 0,   0.0f, 1.0f,
                // top face
                -0.5f, 0.5f, 0,   0, 1.0f, 0,   0.0f, 1.0f,  // triangle
                -0.5f, 0.5f, -1,  0, 1.0f, 0,   1.0f, 1.0f,  
                0.5f, 0.5f, 0,    0, 1.0f, 0,   0.0f, 0.0f,  
                0.5f, 0.5f, -1,   0, 1.0f, 0,   1.0f, 0.0f,  // triangle
                0.5f, 0.5f, 0,    0, 1.0f, 0,   0.0f, 0.0f,
                -0.5f, 0.5f, -1,  0, 1.0f, 0,   1.0f, 1.0f,
                // bottom face
                -0.5f, -0.5f, 0,   0, -1.0f, 0,   1.0f, 1.0f,  // triangle
                -0.5f, -0.5f, -1,  0, -1.0f, 0,   0.0f, 1.0f,  
                0.5f, -0.5f, 0,    0, -1.0f, 0,   1.0f, 0.0f,  
                0.5f, -0.5f, -1,   0, -1.0f, 0,   0.0f, 0.0f,  // triangle
                0.5f, -0.5f, 0,    0, -1.0f, 0,   1.0f, 0.0f,  
                -0.5f, -0.5f, -1,  0, -1.0f, 0,   0.0f, 1.0f
            };
            const u_long componentsSize = 8 * sizeof(float);
            Mesh* mesh(nullptr);
            Texture mainTex("./../../assets/oldman_meme.jpg", Texture::TexFormat::RGB, Texture::PixelFormat::RGB);
            Shader shader(SRC_VERTEX, SRC_FRAGMENT), 
                   shaderLight(SRC_VERTEX, SRC_FRAGMENT_LIGHT);
            // enable ZTest buffering!
            Enable(WndBuffer::Depth);

            for (int i(0); i < NUM_CUBES; i++) {
                mesh = new Mesh(rect, sizeof(rect), componentsSize);

                // position attribute
                mesh->SetAttribute(0, 3, GL_FLOAT, false, componentsSize, 0);
                // normals
                mesh->SetAttribute(1, 3, GL_FLOAT, false, componentsSize, 3 * sizeof(float));
                // uv's attribute
                mesh->SetAttribute(2, 2, GL_FLOAT, false, componentsSize, 6 * sizeof(float));
                
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
            shader.SetFloat("ambientFactor", AMBIENT_FACTOR);
            shader.SetVec3("lightPosition", LIGHT_POSITION);
            CreateLight(rect, sizeof(rect), componentsSize);
        }

        void OnRenderLight(const glm::mat4& view, const glm::mat4& proj) {
            glm::mat4 model(1.0f);

            model = glm::translate(model, LIGHT_POSITION);
            model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));

            lightSource->Render();
            lightSource->GetShader().SetMat4x4("model", model);
            lightSource->GetShader().SetMat4x4("view", view);
            lightSource->GetShader().SetMat4x4("projection", proj);
            lightSource->GetShader().SetVec3("lightColor", LIGHT_COLOR);
            lightSource->Draw();
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
            cubes.at(0)->GetShader().SetVec3("cameraPos", mainCamera.GetPosition());
            
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
                mesh->Draw();
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
        const float cameraSpeed = 0.1f, sensitivity = 0.1f, AMBIENT_FACTOR = 0.25f;
        const glm::vec3 LIGHT_COLOR = glm::vec3(1.0f, 1.0f, 1.0f),
                        OBJECT_COLOR = glm::vec3(1.0f, 0.5f, 0.31f),
                        LIGHT_POSITION = glm::vec3(1.0f, 1.0f, 1.0f);
};

int main() {
    CameraMove wnd("LearnOpenGL", 800, 600);
    wnd.MainLoop();
    return 0;
}