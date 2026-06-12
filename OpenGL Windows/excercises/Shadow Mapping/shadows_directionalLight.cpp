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
#include "scene/iscene_renderable.hpp"

#include "scene/scene.hpp"

#include "model_import/model_importer.hpp"
#include "ui_utils/ui_transform.hpp"
#include "ui_utils/ui_boolean_value.hpp"

#include "../primitives/Quad.hpp"

#include "frame_buffers/frame_buffer_quad.hpp"
#include "frame_buffers/depth_frame_buffer.hpp"
#include "shading/cube_map.hpp"

#include "Utils.hpp"

using namespace CEngine;

using json = nlohmann::json;

/**
 * Compile this with cmake (in root folder)
*/

class ShadowsDirectionalLights : public Window {
public:
    ShadowsDirectionalLights(const std::string& wnd, const int& w, const int h, const json& jsonConfig)
        : Window(wnd, w, h), mainCamera(), mainScene(), showBothRenders(false)
    {
        RetrieveSettings(jsonConfig);

        InitFrameBuffers();

        CreateSkybox();

        // init scene
        mainScene.InitializeGlobalData();
    }

    ~ShadowsDirectionalLights() {

        std::cout << "[Framebuffer_Basics::FreesMemory]" << std::endl;

    }

    void RetrieveSettings(const json& jsonConfig) {

        settings.CamFOV = jsonConfig["CameraSettings"]["FOV"];
        settings.CamMovementSpeed = jsonConfig["CameraSettings"]["MovementSpeed"];
        settings.CamLookSpeed = jsonConfig["CameraSettings"]["LookSpeed"];
        settings.CamNear = jsonConfig["CameraSettings"]["NearPlane"];
        settings.CamFar = jsonConfig["CameraSettings"]["FarPlane"];

        settings.WindowWidth = jsonConfig["Window"]["Width"];
        settings.WindowHeight = jsonConfig["Window"]["Height"];

        // models
        settings.BunnyModelPath = 
            Assets::GetAssetPath(jsonConfig["ModelAssets"]["Model3D"]);
        // textures
        settings.BunnyTexturePath = 
            Assets::GetAssetPath(jsonConfig["ModelAssets"]["BunnyTexture"]);
        
        settings.WindowTexturePath = 
            Assets::GetAssetPath(jsonConfig["ModelAssets"]["WindowTexture"]);
    
        settings.FloorTexturePath = 
            Assets::GetAssetPath(jsonConfig["ModelAssets"]["FloorTexture"]);

        // shaders
        settings.ModelShaderVertex = 
            Assets::GetShaderCode(jsonConfig["ModelShader"]["Vertex"]);
        settings.ModelShaderFrag = 
            Assets::GetShaderCode(jsonConfig["ModelShader"]["Frag"]);

        settings.ScreenShaderVertex = 
            Assets::GetShaderCode(jsonConfig["ScreenShader"]["Vertex"]);
        settings.ScreenShaderFrag = 
            Assets::GetShaderCode(jsonConfig["ScreenShader"]["Frag"]);

        // Depth shaders
        settings.DepthModelVertexShader =
            Assets::GetShaderCode(jsonConfig["DepthModelShader"]["Vertex"]);
        settings.DepthModelFragShader =
            Assets::GetShaderCode(jsonConfig["DepthModelShader"]["Frag"]);

        settings.DebugScreenShaderVertex = 
            Assets::GetShaderCode(jsonConfig["DebugScreenShader"]["Vertex"]);
        settings.DebugScreenShaderFrag =
            Assets::GetShaderCode(jsonConfig["DebugScreenShader"]["Frag"]);
        
        // skybox
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

    void OnMainLoopInit() {
        // enable ZTest buffering!
        Enable(WndBuffer::Depth);
        
        ImportModel();

        CreateWindowQuad();

        CreateFloorQuad();

        CreateLights();

        InitShadows();

        // link UI controls to the model
        transformWnd.WindowName = "Bunny Model";
        transformWnd.Context(mainScene.GetChild(0));
        sunTransformWnd.WindowName = "Sun Directional Light";
        sunTransformWnd.Context(sun);
        switchToDepthWnd.WindowName = "Display Both Renders?";
        switchToDepthWnd.Attach("Switch", [this]() {
            showBothRenders = !showBothRenders;
        });
    }

    void InitFrameBuffers() {

        float w(settings.WindowWidth), h(settings.WindowHeight);

        screenFrameBuffer = std::make_unique<FrameBufferQuad>(
            w, h, std::make_shared<Shader>(settings.ScreenShaderVertex, settings.ScreenShaderFrag)
        );

        depthFrameBuffer = std::make_unique<DepthFrameBuffer>(1024, 1024);

        depthFrameBufferShader = std::make_shared<Material>(
            settings.DepthModelVertexShader, settings.DepthModelFragShader
        );

        debugScreenFrameBufferShader = std::make_shared<Shader>(
            settings.DebugScreenShaderVertex, settings.DebugScreenShaderFrag
        );
        debugScreenFrameBuffer = std::make_unique<FrameBufferQuad>(
            w, h, debugScreenFrameBufferShader
        );
    }
    
    void CreateSkybox() {

        std::shared_ptr<Material> skyMat;
        skyMat = std::make_shared<Material>(
            settings.SkyboxShaderVertex, settings.SkyboxShaderFrag
        );

        backgroundSkybox = std::make_unique<CubeMap>(
            settings.SkyBoxImgs, Texture::TexColorSpace::GAMMA
        );
        backgroundSkybox->Skybox(skyMat);
    }

    void ImportModel() {
        ModelImporter importer;
        auto shader = 
            std::make_shared<Shader>(settings.ModelShaderVertex, settings.ModelShaderFrag);
        auto diffuseTex = std::make_shared<Texture>(
            settings.BunnyTexturePath, Texture::ImgType::JPEG, Texture::TexColorSpace::GAMMA
        );

        // enable ZTest buffering!
        Enable(WndBuffer::Depth);
        // import the model
        importer.BaseShader(shader);
        importer.Import(settings.BunnyModelPath);

        // retrieve the data
        for (Actor* actor : importer.Data()) {
            // to bind with current VAO
            Logger::Log("READS MODEL =>" + actor->Name);
            actor->SetScale(0.005f);
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

        sun = std::make_shared<DirectionalLight>(
            Color::From255(Vector3(255.0f, 87.0f, 51.0f)),
            Vector3(1, 1, 1),   // pos
            Vector3(-35, 25, 0) // rotation
        );

        pLight->SetPosition(Vector3(-0.5, 0.5, -0.5));
        pLight->SetColor(Color::Blue);
        pLight2->SetPosition(Vector3(0.5, -0.5, 0.5));
        pLight2->SetColor(Color::Green);
        pLight3->SetPosition(Vector3(-0.5, -0.5, 0.5));
        pLight3->SetColor(Color::From255(255, 192, 203)); // pink

        mainScene.AddPointLight(pLight);
        mainScene.AddPointLight(pLight2);
        mainScene.AddPointLight(pLight3);
    }

    void CreateWindowQuad() {
        auto shader = 
            std::make_shared<Shader>(settings.ModelShaderVertex, settings.ModelShaderFrag);
        auto windowTex = std::make_shared<Texture>(
            settings.WindowTexturePath, Texture::ImgType::PNG, Texture::TexColorSpace::GAMMA
        );
        std::shared_ptr <Material> newMat = std::make_shared<Material>(
            settings.ModelShaderVertex, settings.ModelShaderFrag
        );
        newMat->InnerShader()->ShaderType(Shader::Type::Transparent);

        auto newQuad = std::make_shared<Primitives::Quad>("Window 1", newMat);

        newMat->AddTexture(windowTex, "Window Texture");

        newQuad->SetPosition(Vector3(0, 0, -0.5));

        mainScene.AddActor(newQuad);

    }

    void CreateFloorQuad() {
        auto floorTex = std::make_shared<Texture>(
            settings.FloorTexturePath, Texture::ImgType::JPEG, Texture::TexColorSpace::GAMMA
        );
        auto newMat = std::make_shared<Material>(
            settings.ModelShaderVertex, settings.ModelShaderFrag
        );

        auto newQuad = std::make_shared<Primitives::Quad>("Floor", newMat);

        newMat->AddTexture(floorTex, "Floor Texture");

        newQuad->SetPosition(Vector3(0, -0.6f, 0));
        newQuad->RotateX(-90.0f);
        newQuad->SetScale(3);

        mainScene.AddActor(newQuad); 
        
        blinnPhongShader = newMat;
    }

    void InitShadows() {

        mainScene.ForEachObject([this](const std::shared_ptr<Object>& obj) {
        
            obj->material()->AddTexture(depthFrameBuffer->Tex(), "depthTexture");
        });
    }

    void RenderDepthBuffer() {
        Matrix4 proj, view;
        Vector3 sunPos = sun->GetPosition(), lookPos = sun->Transform().Forward();
        const float nearPlane(0.01), farPlane(7.5f);

        proj = Matrix4::Ortho(
            -3.0f, 3.0f, -3.0f, 3.0f, nearPlane, farPlane
        );
        view = Matrix4::LookAt(sunPos, sunPos + lookPos, Vector3::Up());

        depthFrameBuffer->Render([this, &proj, &view]() {

            mainScene.RenderWithShader(view, proj, depthFrameBufferShader);
        });

        // DEBUG for showing the depth texture to the screen
        debugScreenFrameBuffer->RenderQuad(false, [this]() {
            // bind image with texture set 0 + N
            depthFrameBuffer->UseDepthTextureInShader();
        });
    }

    void RenderSceneNormally() {
        // transformations
        Matrix4 proj, view = mainCamera.GetViewMatrix();

        proj = Matrix4::Perspective(
            settings.CamFOV, (float)GetAspectRatio(), settings.CamNear, settings.CamFar
        );

        screenFrameBuffer->UseAsRenderTarget();

        // render the whole scene to screen texture
        mainScene.Render(view, proj, backgroundSkybox.get());

        // display the final screen from framebuffer
        screenFrameBuffer->RenderQuad();
    }

    virtual void HandleUI() {
        // transform window
        transformWnd.UpdateUI();
        sunTransformWnd.UpdateUI();
        switchToDepthWnd.UpdateUI();
    }

    void OnRender() {
        
        mainScene.Sun(*sun);

        RenderDepthBuffer();
        if (showBothRenders)
            RenderSceneNormally();
    }

private:
    Scene mainScene;
    Camera mainCamera;
    // needed for shadows
    std::shared_ptr<DirectionalLight> sun;
    std::unique_ptr<DepthFrameBuffer> depthFrameBuffer;
    std::shared_ptr<Material> depthFrameBufferShader;
    std::shared_ptr<Material> blinnPhongShader;
    // shadows and depth debug
    std::unique_ptr<FrameBufferQuad> debugScreenFrameBuffer;
    std::shared_ptr<Shader> debugScreenFrameBufferShader;

    // post-processing effects
    std::unique_ptr<FrameBufferQuad> screenFrameBuffer;
    std::unique_ptr<CubeMap> backgroundSkybox;

    // ui...
    UITransform transformWnd, sunTransformWnd;
    UIBooleanValue switchToDepthWnd;
    bool showBothRenders;

    // config file settings
    struct Settings {
        int CamFOV, WindowWidth, WindowHeight;   
        float CamNear, CamFar, CamLookSpeed, CamMovementSpeed;

        std::string BunnyModelPath, ModelShaderVertex, ModelShaderFrag;

        std::string BunnyTexturePath, WindowTexturePath, FloorTexturePath;

        std::string ScreenShaderVertex, ScreenShaderFrag;

        std::string DepthModelVertexShader, DepthModelFragShader;

        std::string DebugScreenShaderVertex, DebugScreenShaderFrag;

        std::vector<std::string> SkyBoxImgs;
        std::string SkyboxShaderVertex, SkyboxShaderFrag;

    } settings;

};


// NOTE: file paths are relative to the 
//  console's location, which is in the "out/..." folder!!!

int main() {
    std::ifstream jsonFile(
        Assets::GetConfigFilePath("Shadow Mapping/shadows_dir_light.json")
    );
    json configData = json::parse(jsonFile);

    std::cout << "Current PATH => " << std::filesystem::current_path() << std::endl;

    ShadowsDirectionalLights wnd(
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