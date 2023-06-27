#include <iostream>
#include <memory>
#include <vector>
#include <stdlib.h>
#include <time.h>
#include <filesystem>
#include <cstring>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "window.hpp"
#include "controllers/camera.hpp"

#include "shading/Texture.hpp"

#include "scene/actor.hpp"
#include "scene/spotlight.hpp"

#include "ShaderFile.hpp"

#include "scene/scene.hpp"

using namespace CEngine;

/**
 * Compile this with cmake (in root folder)
*/
struct LightData {
    Color ambient, specular;
    SpotLight data;
    Vector3 viewPos, forward;
};

struct CubeData {
    float shininess;
};

float getRandom() {
    int r = rand() % 10;
    return (float)r / 10.0f;
}

std::string rootFolder(const std::string& asset) {
    return "./../../../../" + asset;
}

std::string readShader(std::string file) {
    ShaderFile s(file);

    return s.Code();
}

class SpotLightExercise : public Window {
    public:
        SpotLightExercise(const std::string& wnd, const int& w, const int h)
            : Window(wnd, w, h), mainCamera() {
                // init light data
                light.ambient = Color(0.5f, 0.5f, 0.5f);
                light.specular = Color::White;
                light.data.SpotAngle(glm::cos(glm::radians(12.5f)));
                // init random cube's material properties
                cubeData.shininess = 128.0f;
            }

        ~SpotLightExercise() {
            std::cout << "[Multiple-Cubes::FreesMemory]" << std::endl;
        }

        void CreateLight() {
            // there is no mesh for the light now
            light.data.Name = "Name";;
            // set white as color
            light.data.SetColor(Color::White);
            light.ambient = Color(AMBIENT_FACTOR, AMBIENT_FACTOR, AMBIENT_FACTOR);
        }
        
        void UpdateMouseLook() {
            Vector3 rot(0.0f);
            // jaw
            if (Input::PressedLEFT())
                rot.SetY(360.0f * Time::deltaTime * sensitivity);
            if (Input::PressedRIGHT())
                rot.SetY(-360.0f * Time::deltaTime * sensitivity);
            // pitch
            if (Input::PressedUP())
                rot.SetX(360.0f * Time::deltaTime * sensitivity);
            if (Input::PressedDOWN())
                rot.SetX(-360.0f * Time::deltaTime * sensitivity);

            mainCamera.RotateYaw(rot.Y());
            mainCamera.RotatePitch(rot.X());
        }

        void HandleInput() {
            Vector3 dir(0);
            if (Input::PressedW())
                // move forward (since camera faces towards negative z)
                dir = mainCamera.Transform().Forward();
            if (Input::PressedS())
                // move backwards (camera faces towards negative z)
                dir = -mainCamera.Transform().Forward();
            if (Input::PressedA())
                // move left
                dir = -mainCamera.Transform().Right();
            if (Input::PressedD())
                // move right
                dir = mainCamera.Transform().Right();

            mainCamera.Move(dir * Time::deltaTime);

            if (Input::PressedESC())
                Close();

            UpdateMouseLook();
        }

        void HandleUI() {
            
            static float f = 0.0f;
            static bool show_demo_window = true,
                        show_another_window = false;
            static int counter = 0;
            static ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

            ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.


            ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
            ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
            ImGui::Checkbox("Another Window", &show_another_window);

            ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
            ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

            if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
                counter++;
            ImGui::SameLine();
            ImGui::Text("counter = %d", counter);

            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / Framerate(), Framerate());
            ImGui::End();

            // 3. Show another simple window.
            if (show_another_window)
            {
                ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
                ImGui::Text("Hello from another window!");
                if (ImGui::Button("Close Me"))
                    show_another_window = false;
                ImGui::End();
            }
            
        }

        /**
         * Excercise is to create basic phong lighting in view space
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
            Material* material = new Material(SRC_VERTEX, SRC_FRAG);
            Mesh* mesh;
            // since these are PNGs
            CEngine::Texture diffuseTex(DIFFUSE_MAP, CEngine::Texture::ImgType::PNG),
                    specularTex(SPECULAR_MAP, CEngine::Texture::ImgType::PNG),
                    // since is JPG
                    emissionTex(EMISSION_MAP, CEngine::Texture::ImgType::JPEG);
            
            // enable ZTest buffering!
            Enable(WndBuffer::Depth);

            mesh = new Mesh(rect, sizeof(rect), componentsSize);

            // position attribute
            mesh->SetAttribute(0, 3, GL_FLOAT, false, componentsSize, 0);
            // normals
            mesh->SetAttribute(1, 3, GL_FLOAT, false, componentsSize, 3 * sizeof(float));
            // uv's attribute
            mesh->SetAttribute(2, 2, GL_FLOAT, false, componentsSize, 6 * sizeof(float));
            
            // to bind with current VAO
            mesh->Use();
            material->Use();
            // bind diffuse with texture set 0
            glActiveTexture(GL_TEXTURE0); 
            diffuseTex.Use();
            material->SetUniform("diffuseTexture", 0);
            // bind diffuse with texture set 1
            glActiveTexture(GL_TEXTURE1);
            specularTex.Use();
            material->SetUniform("specularTexture", 1);
            // bind diffuse with texture set 1
            glActiveTexture(GL_TEXTURE2);
            emissionTex.Use();
            material->SetUniform("emissionTexture", 2);
            
            // put here to make sure VAO's been binded
            material->SetTexture(diffuseTex);
            material->SetTexture(specularTex);
            material->SetTexture(emissionTex);
            
            Logger::Log("Initializes the data =>");

            mainScene.InitializeGlobalData(
                UNIFORM_BLOCK_NAME, 
                UNIFORM_BLOCK_INDEX,
                Matrix4::Size() * 2 // since there are two matrices (view, proj)
            );
            cube = std::make_shared<Actor>( "Cube", mesh, material );
            mainScene.AddActor( cube );
            CreateLight();
        }

        void OnRenderLight(const Matrix4& view, const Matrix4& proj) {
            Vector4 lightPosViewSpace, forward;
            const Transform& camT = mainCamera.Transform();

            // stick the light to the camera
            light.data.Transform().Position(camT.Position());
            light.data.Transform().Rotation(camT.Rotation());
            forward = Vector4(camT.Forward(), 0.0f);

            // send pos to cubes shader in view space (reused later in OnRender)
            lightPosViewSpace = Matrix4::Multiply(
                    view * camT.ModelMatrix(), 
                    Vector4(camT.Position(), 1.0f)
            );
            
            // send "forward" to cubes shader in view space (reused later in OnRender)
            forward = Matrix4::Multiply(
                    view * camT.ModelMatrix(), 
                    forward
            );
            light.viewPos = lightPosViewSpace.ToVector3();
            light.forward = forward.ToVector3();
        }

        void OnRender() {
            // transformations
            Matrix4 proj, view = mainCamera.GetViewMatrix();

            proj = Matrix4::Perspective(45.0f, (float)GetAspectRatio(), 0.1f, 100.0f);
            
            OnRenderLight(view, proj);

            mainScene.Render(view, proj);

            cube->BeginRender();
            // update uniforms
            cube->material()->SetUniform("material.shininess", cubeData.shininess);
            
            cube->material()->SetUniform("light.position", light.viewPos);
            cube->material()->SetUniform("light.ambient", light.ambient);
            cube->material()->SetUniform("light.specular", light.specular);
            cube->material()->SetUniform("light.color", light.data.GetColor());
            //cube->SetUniform<float>("light.k1", light.data.K1());
            //cube->SetUniform<float>("light.kq", light.data.Kq());
            cube->material()->SetUniform("light.maxAngle", light.data.SpotAngle());
            cube->material()->SetUniform("light.forward", -Vector3::Forward());// light.forward);
            
            cube->EndRender();

            // delta time is updated, internally, by window
        }
    
    private:
        std::shared_ptr<Actor> cube;
        Scene mainScene;
        LightData light;
        CubeData cubeData;
        Camera mainCamera;

        const float cameraSpeed = 0.1f, sensitivity = 0.1f;
        const float LIGHT_RADIUS_ROT = 2, LIGHT_SPEED = 1, AMBIENT_FACTOR = 0.25f;
        
        // NOTE: these paths is relative to the 
        //  console's location which is in the "out/..." folder!!!
        // shaders...
        const std::string SRC_VERTEX = readShader(rootFolder("Shaders/VertexBasic.shader")),
                          SRC_FRAG = readShader(rootFolder("Shaders/Lights exercise/Spot_Light.shader"));
        // textures
        const std::string DIFFUSE_MAP = rootFolder("assets/box-container.png"),
                          SPECULAR_MAP = rootFolder("assets/box-container-specular.png"),
                          EMISSION_MAP = rootFolder("assets/matrix_emission.jpg");

        const std::string UNIFORM_BLOCK_NAME = "GlobalMatrices";
        const int UNIFORM_BLOCK_INDEX = 0;
};


int main() {

    std::cout << "Current PATH => " << std::filesystem::current_path() << std::endl;

    SpotLightExercise wnd("LearnOpenGL => Spot light", 800, 600);

    // init random seed
    srand(time(nullptr));

    wnd.MainLoop();
    return 0;
}