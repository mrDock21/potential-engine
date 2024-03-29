#include <iostream>
#include <memory>
#include <vector>
#include <stdlib.h>
#include <time.h>
#include <filesystem>
#include <cstring>

#include "window.hpp"
#include "controllers/camera.hpp"

#include "scene/actor.hpp"
#include "scene/pointlight.hpp"

#include "ShaderFile.hpp"

/**
 * Compile this with cmake (in root folder)
*/
const std::string SRC_FRAGMENT_LIGHT =
    "#version 330 core\n"
    "out vec4 FragColor;\n"
    "uniform vec4 lightColor;\n"
    "void main() {\n"
    "   FragColor = lightColor;\n"
    "}\n";

struct LightData {
    Color ambient, specular;
    std::unique_ptr<Actor> model;
    PointLight data;
    Vector3 viewPos;
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

class PointLightExercise : public Window {
    public:
        PointLightExercise(const std::string& wnd, const int& w, const int h)
            : Window(wnd, w, h), mainCamera() {
                // init light data
                light.ambient = Color(0.5f, 0.5f, 0.5f);
                light.specular = Color::White;
                light.data.K1(0.09f);
                light.data.Kq(0.032f);
                // init random cube's material properties
                cubeData.shininess = 128.0f;
            }

        ~PointLightExercise() {
            std::cout << "[Multiple-Cubes => FreesMemory]" << std::endl;
        }

        void CreateLight(float* vertices, const u_long& vsize, const u_long& esize) {
            Mesh* mesh = new Mesh(vertices, vsize, esize);
            Material* mat = new Material(SRC_VERTEX, SRC_FRAGMENT_LIGHT);

            light.model = std::unique_ptr<Actor>( new Actor("Light", mesh, mat) );
            
            mesh->Use();
            // position attribute
            mesh->SetAttribute(0, 3, GL_FLOAT, false, esize, 0);
            // normals
            mesh->SetAttribute(1, 3, GL_FLOAT, false, esize, 3 * sizeof(float));
            // uv's attribute
            mesh->SetAttribute(2, 2, GL_FLOAT, false, esize, 6 * sizeof(float));
            // to make sure they are binded them
            mesh->Use();
            mat->Use();

            light.model->Transform().SetPosition(Vector3(1));
            light.model->Transform().SetScale(0.5f);

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

            if (Input::PressedQ())
                light.model->Transform().Translate(Vector3::Forward() * Time::deltaTime);
            else if (Input::PressedE())
                light.model->Transform().Translate(-Vector3::Forward() * Time::deltaTime);

            UpdateMouseLook();
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
            Texture diffuseTex(DIFFUSE_MAP, Texture::ImgType::PNG),
                    specularTex(SPECULAR_MAP, Texture::ImgType::PNG),
                    // since is JPG
                    emissionTex(EMISSION_MAP, Texture::ImgType::JPEG);
            
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
            material->SetUniform<int>("diffuseTexture", 0);
            // bind diffuse with texture set 1
            glActiveTexture(GL_TEXTURE1);
            specularTex.Use();
            material->SetUniform<int>("specularTexture", 1);
            // bind diffuse with texture set 1
            glActiveTexture(GL_TEXTURE2);
            emissionTex.Use();
            material->SetUniform<int>("emissionTexture", 2);
            
            // put here to make sure VAO's been binded
            material->SetTexture(diffuseTex);
            material->SetTexture(specularTex);
            material->SetTexture(emissionTex);
            
            cube = std::unique_ptr<Actor>( new Actor("Cube", mesh, material) );
            
            CreateLight(rect, sizeof(rect), componentsSize);
        }

        void OnRenderLight(const Matrix4& view, const Matrix4& proj) {
            Matrix4 model = Matrix4::Indentity();
            Vector4 lightPosViewSpace;

            // send pos to cubes shader in view space (reused later in OnRender)
            lightPosViewSpace = Matrix4::Multiply(
                    view * light.model->Transform().ModelMatrix(), 
                    Vector4(light.model->Transform().Position(), 1.0f)
            );
            light.viewPos = lightPosViewSpace.ToVector3();

            // for light's shader   
            light.model->BeginRender(view, proj);
            light.model->SetUniform<Color>("lightColor", light.data.GetColor());
            light.model->EndRender();
        }

        void OnRender() {
            // transformations
            Matrix4 proj, view = mainCamera.GetViewMatrix();

            proj = Matrix4::Perspective(45.0f, (float)GetAspectRatio(), 0.1f, 100.0f);
            
            OnRenderLight(view, proj);

            cube->BeginRender(view, proj);
            // update uniforms
            cube->SetUniform<float>("material.shininess", cubeData.shininess);
            
            cube->SetUniform<Vector3>("light.position", light.viewPos);
            cube->SetUniform<Color>("light.ambient", light.ambient);
            cube->SetUniform<Color>("light.specular", light.specular);
            cube->SetUniform<Color>("light.color", light.data.GetColor());
            cube->SetUniform<float>("light.k1", light.data.K1());
            cube->SetUniform<float>("light.kq", light.data.Kq());
            
            cube->EndRender();

            // delta time is updated, internally, by window
        }
    
    private:
        std::unique_ptr<Actor> cube;
        LightData light;
        CubeData cubeData;
        Camera mainCamera;

        const float cameraSpeed = 0.1f, sensitivity = 0.1f;
        const float LIGHT_RADIUS_ROT = 2, LIGHT_SPEED = 1, AMBIENT_FACTOR = 0.25f;
        
        // NOTE: these paths is relative to the 
        //  console's location which is in the "out/..." folder!!!
        // shaders...
        const std::string SRC_VERTEX = readShader(rootFolder("Shaders/VertexBasic.shader")),
                          SRC_FRAG = readShader(rootFolder("Shaders/Lights exercise/Point_Light.shader"));
        // textures
        const std::string DIFFUSE_MAP = rootFolder("assets/box-container.png"),
                          SPECULAR_MAP = rootFolder("assets/box-container-specular.png"),
                          EMISSION_MAP = rootFolder("assets/matrix_emission.jpg");
};


int main() {

    std::cout << "Current PATH => " << std::filesystem::current_path() << std::endl;

    PointLightExercise wnd("LearnOpenGL => Point light", 800, 600);

    // init random seed
    srand(time(nullptr));

    wnd.MainLoop();
    return 0;
}