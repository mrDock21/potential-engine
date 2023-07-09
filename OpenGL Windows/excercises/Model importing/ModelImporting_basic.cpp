#include <iostream>
#include <memory>
#include <vector>
#include <stdlib.h>
#include <time.h>
#include <filesystem>
#include <cstring>

#include "window.hpp"
#include "controllers/camera.hpp"

#include "shading/Texture.hpp"

#include "scene/actor.hpp"
#include "scene/spotlight.hpp"

#include "ShaderFile.hpp"

#include "scene/scene.hpp"

#include "model_import/model_importer.hpp"
#include "ui_utils/ui_transform.hpp"


using namespace CEngine;

/**
 * Compile this with cmake (in root folder)
*/

float getRandom() {
    int r = rand() % 10;
    return (float)r / 10.0f;
}

std::string rootFolder(const std::string& asset) {
    return "./../../../../" + asset;
}

std::string assets(const std::string& asset) {
    return "./../../../../assets/" + asset;
}

std::string readShader(std::string file) {
    ShaderFile s(file);

    return s.Code();
}

class Model_Importing_Basic : public Window {
    public:
        Model_Importing_Basic(const std::string& wnd, const int& w, const int h)
            : Window(wnd, w, h), mainCamera() 
        {
            // init scene
            mainScene.InitializeGlobalData();
        }

        ~Model_Importing_Basic() {
            std::cout << "[Model_Importing_Basic::FreesMemory]" << std::endl;
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

        virtual void HandleUI() {
            // transform window
            transformWnd.UpdateUI();
        }

        /**
         * Excercise is to create basic phong lighting in view space
        */
        void OnMainLoopInit() {
            ModelImporter importer;
            std::shared_ptr<Shader> shader = std::make_shared<Shader>(SRC_VERTEX, SRC_FRAG);
            CEngine::Texture diffuseTex(DIFFUSE_MAP, CEngine::Texture::ImgType::JPEG);
            
            // enable ZTest buffering!
            Enable(WndBuffer::Depth);
            // import the model
            importer.BaseShader(shader);
            importer.Import(MODEL_PATH);
            // retrieve the data
            for (Actor* actor : importer.Data()) {
                // to bind with current VAO
                Logger::Log("READS MODEL =>" + actor->Name);
                actor->Transform().Scale(0.005f);
                actor->mesh()->Use();
                actor->material()->AddTexture(diffuseTex, "diffuseTexture");
                std::shared_ptr<Actor> a(actor);
                mainScene.AddObject(a);
            }
            Logger::Log("Initializes the data =>");
            
            auto pLight = std::make_shared<PointLight>();
            auto pLight2 = std::make_shared<PointLight>();
            auto pLight3 = std::make_shared<PointLight>();
            pLight->Transform().Position(Vector3(-0.5, 0.5, -0.5));
            pLight->SetColor(Color::Blue);
            pLight2->Transform().Position(Vector3(0.5, -0.5, 0.5));
            pLight2->SetColor(Color::Green);
            pLight3->Transform().Position(Vector3(-0.5, -0.5, 0.5));
            pLight3->SetColor(Color::From255(255, 192, 203)); // pink

            mainScene.AddPointLight(pLight);
            mainScene.AddPointLight(pLight2);
            mainScene.AddPointLight(pLight3);

            transformWnd.Context(mainScene.GetChild(0));
        }

        // static version
        void OnMainLoopInit2() {
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
            CEngine::Texture diffuseTex(DIFFUSE_MAP, CEngine::Texture::ImgType::JPEG);

            // enable ZTest buffering!
            Enable(WndBuffer::Depth);

            mesh = new Mesh(rect, sizeof(rect), componentsSize);

            mesh->Use();
            // position attribute
            mesh->SetAttribute(0, 3, GL_FLOAT, false, componentsSize, 0);
            // normals
            mesh->SetAttribute(1, 3, GL_FLOAT, false, componentsSize, 3 * sizeof(float));
            // uv's attribute
            mesh->SetAttribute(2, 2, GL_FLOAT, false, componentsSize, 6 * sizeof(float));
            material->Use();
            material->AddTexture(diffuseTex, "diffuseTexture");

            Logger::Log("Initializes the data =>");

            auto pLight = std::make_shared<PointLight>();
            auto pLight2 = std::make_shared<PointLight>();
            auto pLight3 = std::make_shared<PointLight>();
            pLight->Transform().Position(Vector3(-1, 1, 0));
            pLight->SetColor(Color::Blue);
            pLight2->Transform().Position(Vector3(1, -1, 0));
            pLight2->SetColor(Color::Green);
            pLight3->Transform().Position(Vector3(0, 0, -1));
            pLight3->SetColor(Color::From255(255, 255, 255));

            mainScene.AddPointLight(pLight);
            mainScene.AddPointLight(pLight2);
            mainScene.AddPointLight(pLight3);

            auto ptr = std::make_shared<Actor>("Cube", mesh, material);
            mainScene.AddObject(ptr);

            transformWnd.Context(ptr);
        }

        void OnRender() {
            // transformations
            Matrix4 proj, view = mainCamera.GetViewMatrix();

            proj = Matrix4::Perspective(45.0f, (float)GetAspectRatio(), 0.1f, 100.0f);
            
            mainScene.Render(view, proj);

            // delta time is updated, internally, by window
        }
    
    private:
        Scene mainScene;
        Camera mainCamera;

        // ui...
        UITransform transformWnd;

        const float cameraSpeed = 0.1f, sensitivity = 0.1f;
        const float LIGHT_RADIUS_ROT = 2, LIGHT_SPEED = 1, AMBIENT_FACTOR = 0.25f;
        
        // NOTE: these paths are relative to the 
        //  console's location, which is in the "out/..." folder!!!
        // shaders...
        const std::string SRC_VERTEX = readShader(rootFolder("Shaders/MultipleLights_Vertex.shader")),
                          SRC_FRAG = readShader(rootFolder("Shaders/Model importing/MIBasic.shader"));
        // model
        const std::string MODEL_PATH = assets("stanford-bunny/model.dae");
        // textures
        const std::string DIFFUSE_MAP = 
            assets("stanford-bunny/textures/DefaultMaterial_diffuse.jpg");

};


int main() {

    std::cout << "Current PATH => " << std::filesystem::current_path() << std::endl;

    Model_Importing_Basic wnd("LearnOpenGL => Importing model", 800, 600);

    // init random seed
    srand(time(nullptr));

    wnd.MainLoop();
    return 0;
}