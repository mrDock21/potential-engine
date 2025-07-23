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

#include "scene/scene.hpp"

#include "model_import/model_importer.hpp"
#include "ui_utils/ui_transform.hpp"

#include "../primitives/Quad.hpp"

#include "Utils.hpp"

using namespace CEngine;

/**
 * Compile this with cmake (in root folder)
*/

class Blending_Basic : public Window {
public:
    Blending_Basic(const std::string& wnd, const int& w, const int h)
        : Window(wnd, w, h), mainCamera()
    {
        // init scene
        mainScene.InitializeGlobalData();
    }

    ~Blending_Basic() {
        std::cout << "[Blending_Basic::FreesMemory]" << std::endl;
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
        // enable ZTest buffering!
        Enable(WndBuffer::Depth);

        ImportModel();

        CreateLights();

        CreateQuad();

        // link UI controls to the model
        transformWnd.Context(mainScene.GetChild(0));
    }

    // static version
    void ImportModel() {
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
    }

    void CreateLights() {
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
    }

    void CreateQuad() {
        CEngine::Texture windowTex(WINDOW_DIFFUSE_MAP, CEngine::Texture::ImgType::PNG);
        Material* newMat;
        auto shader = std::make_shared<Shader>(SRC_VERTEX, SRC_FRAG);
        
        shader->ShaderType(Shader::Type::Transparent);
        newMat = new Material(shader);

        auto newQuad = std::make_shared<Primitives::Quad>("Window 1", newMat);

        newMat->AddTexture(windowTex, "Window Texture");

        newQuad->Transform().Position(Vector3(0, 0, -0.5));

        mainScene.AddObject(newQuad);

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
    const std::string SRC_VERTEX = Assets::GetShaderCode(
        Assets::GetShaderPath("MultipleLights_Vertex.shader")
    ),
        SRC_FRAG = Assets::GetShaderCode(
            Assets::GetShaderPath("Model importing/MIBasic.shader")
    );
    // model
    const std::string MODEL_PATH = Assets::GetAssetPath("stanford-bunny/model.dae");
    // textures
    const std::string DIFFUSE_MAP =
        Assets::GetAssetPath("stanford-bunny/textures/DefaultMaterial_diffuse.jpg");
    const std::string WINDOW_DIFFUSE_MAP =
        Assets::GetAssetPath("blending_transparent_window.png");

};


int main() {

    std::cout << "Current PATH => " << std::filesystem::current_path() << std::endl;

    Blending_Basic wnd("LearnOpenGL => Importing model", 800, 600);

    // init random seed
    srand(time(nullptr));

    wnd.MainLoop();
    return 0;
}