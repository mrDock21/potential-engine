#include <iostream>
#include <memory>
#include <vector>
#include <stdlib.h>
#include <time.h>
#include <filesystem>
#include <cstring>

#include <iomanip>
#include <nlohmann/json.hpp>

#include "window.hpp"
#include "controllers/camera.hpp"

#include "shading/Texture.hpp"

#include "scene/actor.hpp"
#include "scene/spotlight.hpp"

#include "scene/scene.hpp"

#include "model_import/model_importer.hpp"
#include "ui_utils/ui_transform.hpp"

#include "../primitives/Quad.hpp"

#include "frame_buffers/frame_buffer_quad.hpp"
#include "shading/cube_map.hpp"

#include "Utils.hpp"

using namespace CEngine;

using json = nlohmann::json;

/**
 * Compile this with cmake (in root folder)
*/

class SkyboxBasic : public Window {
public:
    SkyboxBasic(const std::string& wnd, const int& w, const int h, const json& jsonConfig)
        : Window(wnd, w, h), mainCamera()
    {
        RetrieveSettings(jsonConfig);

        screenFrameBuffer = std::make_unique<FrameBufferQuad>(
            w, h, new Shader(settings.ScreenShaderVertex, settings.ScreenShaderFrag)
        );

        CreateSkybox();

        // init scene
        mainScene.InitializeGlobalData();
    }

    ~SkyboxBasic() {
        std::cout << "[Framebuffer_Basics::FreesMemory]" << std::endl;
    }

    void RetrieveSettings(const json& jsonConfig) {

        settings.CamFOV = jsonConfig["CameraSettings"]["FOV"];
        settings.CamMovementSpeed = jsonConfig["CameraSettings"]["MovementSpeed"];
        settings.CamLookSpeed = jsonConfig["CameraSettings"]["LookSpeed"];
        settings.CamNear = jsonConfig["CameraSettings"]["NearPlane"];
        settings.CamFar = jsonConfig["CameraSettings"]["FarPlane"];

        settings.BunnyModelPath = 
            Assets::GetAssetPath(jsonConfig["ModelAssets"]["Model3D"]);
        settings.BunnyTexturePath = 
            Assets::GetAssetPath(jsonConfig["ModelAssets"]["BunnyTexture"]);
        
        settings.WindowTexturePath = 
            Assets::GetAssetPath(jsonConfig["ModelAssets"]["WindowTexture"]);
    
        settings.ModelShaderVertex = 
            Assets::GetShaderCode(jsonConfig["ModelShader"]["Vertex"]);
        settings.ModelShaderFrag = 
            Assets::GetShaderCode(jsonConfig["ModelShader"]["Frag"]);

        settings.ScreenShaderVertex = 
            Assets::GetShaderCode(jsonConfig["ScreenShader"]["Vertex"]);
        settings.ScreenShaderFrag = 
            Assets::GetShaderCode(jsonConfig["ScreenShader"]["Frag"]);
    
        settings.SkyBoxImgs = Assets::GetFilePaths( jsonConfig["SkyboxTextures"] );
        settings.SkyboxShaderVertex = 
            Assets::GetShaderCode(jsonConfig["SkyboxShader"]["Vertex"]);
        settings.SkyboxShaderFrag = 
            Assets::GetShaderCode(jsonConfig["SkyboxShader"]["Frag"]);

    }

    void UpdateMouseLook() {
        Vector3 rot(0.0f);
        // jaw
        if (Input::PressedLEFT())
            rot.SetY(360.0f * Time::deltaTime * settings.CamLookSpeed);
        if (Input::PressedRIGHT())
            rot.SetY(-360.0f * Time::deltaTime * settings.CamLookSpeed);
        // pitch
        if (Input::PressedUP())
            rot.SetX(360.0f * Time::deltaTime * settings.CamLookSpeed);
        if (Input::PressedDOWN())
            rot.SetX(-360.0f * Time::deltaTime * settings.CamLookSpeed);

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
        if (Input::PressedQ())
            // move up
            dir = Vector3::Up();
        if (Input::PressedE())
            // move down
            dir = -Vector3::Up();
            
        mainCamera.Move(settings.CamMovementSpeed * dir * Time::deltaTime);

        if (Input::PressedESC())
            Close();

        UpdateMouseLook();
    }

    virtual void HandleUI() {
        // transform window
        transformWnd.UpdateUI();
    }

    void OnMainLoopInit() {
        // enable ZTest buffering!
        Enable(WndBuffer::Depth);

        ImportModel();

        CreateLights();

        CreateWindowQuad();

        // link UI controls to the model
        transformWnd.Context(mainScene.GetChild(0));
    }

    // static version
    void CreateSkybox() {

        std::shared_ptr<Material> skyMat;
        skyMat = std::make_shared<Material>(
            settings.SkyboxShaderVertex, settings.SkyboxShaderFrag
        );

        backgroundSkybox = std::make_unique<CubeMap>(settings.SkyBoxImgs);
        backgroundSkybox->Skybox(skyMat);
    }

    void ImportModel() {
        ModelImporter importer;
        auto shader = 
            std::make_shared<Shader>(settings.ModelShaderVertex, settings.ModelShaderFrag);
        auto diffuseTex =
            std::make_shared<Texture>(settings.BunnyTexturePath, Texture::ImgType::JPEG);

        // enable ZTest buffering!
        Enable(WndBuffer::Depth);
        // import the model
        importer.BaseShader(shader);
        importer.Import(settings.BunnyModelPath);

        // retrieve the data
        for (Actor* actor : importer.Data()) {
            // to bind with current VAO
            Logger::Log("READS MODEL =>" + actor->Name);
            actor->Transform().Scale(0.005f);
            actor->mesh()->Use();
            actor->material()->AddTexture(diffuseTex, "diffuseTexture");
            std::shared_ptr<Actor> a(actor);
            mainScene.AddActor(a);
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

    void CreateWindowQuad() {
        auto windowTex =
            std::make_shared<Texture>(settings.WindowTexturePath, Texture::ImgType::PNG);
        auto shader = 
            std::make_shared<Shader>(settings.ModelShaderVertex, settings.ModelShaderFrag);
        Material* newMat;

        shader->ShaderType(Shader::Type::Transparent);
        newMat = new Material(shader);

        auto newQuad = std::make_shared<Primitives::Quad>("Window 1", newMat);

        newMat->AddTexture(windowTex, "Window Texture");

        newQuad->Transform().Position(Vector3(0, 0, -0.5));

        mainScene.AddActor(newQuad);

    }

    void OnRender() {
        // transformations
        Matrix4 proj, view = mainCamera.GetViewMatrix();
        const bool shouldNotClearColors = false;

        proj = Matrix4::Perspective(
            settings.CamFOV, (float)GetAspectRatio(), settings.CamNear, settings.CamFar
        );

        screenFrameBuffer->UseAsRenderTarget();

        // render the whole scene to screen texture
        mainScene.Render(view, proj, backgroundSkybox.get());

        // display the final from framebuffer
        screenFrameBuffer->RenderQuad( shouldNotClearColors );

        // delta time is updated, internally, by window
    }

private:
    Scene mainScene;
    Camera mainCamera;
    // post-processing effects
    std::unique_ptr<FrameBufferQuad> screenFrameBuffer;
    std::unique_ptr<CubeMap> backgroundSkybox;

    // ui...
    UITransform transformWnd;

    // config file settings
    struct Settings {
        int CamFOV;   
        float CamNear, CamFar, CamLookSpeed, CamMovementSpeed;

        std::string BunnyModelPath, ModelShaderVertex, ModelShaderFrag;

        std::string BunnyTexturePath, WindowTexturePath;

        std::string ScreenShaderVertex, ScreenShaderFrag;

        std::vector<std::string> SkyBoxImgs;
        std::string SkyboxShaderVertex, SkyboxShaderFrag;

    } settings;

};


// NOTE: file paths are relative to the 
//  console's location, which is in the "out/..." folder!!!

int main() {
    std::ifstream jsonFile(
        Assets::GetConfigFilePath("Screen Effects/SkyboxBasic_config.json")
    );
    json configData = json::parse(jsonFile);

    std::cout << "Current PATH => " << std::filesystem::current_path() << std::endl;

    SkyboxBasic wnd(
        configData["Window"]["Name"], 
        configData["Window"]["Width"], 
        configData["Window"]["Height"], 
        configData
    );

    // init random seed
    srand(time(nullptr));

    wnd.MainLoop();

    return 0;
}