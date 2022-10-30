#include <iostream>
#include <vector>
#include <stdlib.h>
#include <time.h>
#include <filesystem>

#include "window.hpp"
#include "controllers/camera.hpp"

/**
 * Compile this with cmake (in root folder)
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
    // send output to fragment shader in view space
    "   texCoord = aTexCoord;\n"
    "   normal = (view * model * vec4(aNormal, 0.0)).xyz;\n"
    "   fragPos = view * model * vec4(aPos.xyz, 1.0);\n"
    "   gl_Position = projection * view * model * vec4(aPos.xyz, 1.0);\n"
    "}\n";
const std::string SRC_FRAGMENT =
    "#version 330 core\n"
    "in vec2 texCoord;\n"
    "in vec3 normal;\n"
    "in vec4 fragPos;\n"
    "out vec4 FragColor;\n"
    // textures
    "uniform sampler2D diffuseTexture;\n"
    "uniform sampler2D specularTexture;\n"
    "uniform sampler2D emissionTexture;\n"
    // structs
    "struct Light {\n"
        // position should be in view space
    "   vec3 position, color;\n"
    "   vec3 ambient, diffuse, specular;\n"
    "};\n"
    "struct Material {\n"
    "   float shininess;\n"
    "};\n"
    "uniform Light light;\n"
    "uniform Material material;\n"
    "void main() {\n"
    // diffuse compute
    "   vec3 lightDir = normalize(light.position - fragPos.xyz);\n"
    "   float intensity = clamp(dot(lightDir, normal), 0.0, 1.0);\n"
    // specular compute
    "   vec3 viewDir = normalize(vec3(0) - fragPos.xyz);\n"
    "   float specular = pow( clamp( dot( reflect(-lightDir, normal), viewDir), 0.0, 1.0), material.shininess);\n"
    "   float specularFlag = texture(specularTexture, texCoord).r;\n"
    // emission filter
    "   vec3 emissionColor = texture(emissionTexture, texCoord).rgb;\n"
    "   float emissionFlag = step(0.1, emissionColor.r);\n"
    // color combination
    "   vec3 color, texColor = texture(diffuseTexture, texCoord).rgb;\n"
        // ambient
    "   color = light.ambient * texColor;\n"
        // diffuse
    "   color += light.diffuse * intensity * texColor * light.color;\n"
        // emission
    "   color += (1.0 - step(0.1, specularFlag)) * emissionFlag * emissionColor;\n" 
        // specular
    "   color += light.specular * specularFlag * light.color * specular;\n"
    "   FragColor = vec4(color, 1.0);\n"
    "}\n";

const std::string SRC_FRAGMENT_LIGHT =
    "#version 330 core\n"
    "out vec4 FragColor;\n"
    "uniform vec3 lightColor;\n"
    "void main() {\n"
    "   FragColor = vec4(lightColor, 1.0);\n"
    "}\n";

struct LightData {
    Vector3 ambient, diffuse, specular;
    Vector3 position, color;
};

struct CubeData {
    Components::Transform transform;
    float shininess;
};

float getRandom() {
    int r = rand() % 10;
    return (float)r / 10.0f;
}

class LightMaps : public Window {
    public:
        LightMaps(const std::string& wnd, const int& w, const int h) 
            : Window(wnd, w, h), mainCamera(), 
              deltaTime(0), prevTime(0) {
                // init light data
                lightData.position = Vector3(0.0f);
                lightData.ambient = Vector3(0.5f);
                lightData.diffuse = Vector3(1.0f);
                lightData.specular = lightData.color = Vector3(1.0f);
                // init random cube's material properties
                cubeData.transform.SetPosition(Vector3(0));
                cubeData.transform.SetEulerAngles(Vector3(0));
                cubeData.shininess = 128.0f;
            }

        ~LightMaps() { 
            std::cout << "[Multiple-Cubes::FreesMemory]" << std::endl;
            
            delete cube;
            delete lightSource;
        }

        void CreateLight(float* vertices, const u_long& vsize, const u_long& esize) {
            Material s(SRC_VERTEX, SRC_FRAGMENT_LIGHT);

            lightSource = new Mesh(vertices, vsize, esize);
            lightSource->Use();
            // position attribute
            lightSource->SetAttribute(0, 3, GL_FLOAT, false, esize, 0);
            // normals
            lightSource->SetAttribute(1, 3, GL_FLOAT, false, esize, 3 * sizeof(float));
            // uv's attribute
            lightSource->SetAttribute(2, 2, GL_FLOAT, false, esize, 6 * sizeof(float));
                
            s.Use();
            lightSource->SetMaterial(s);
        }
        
        void UpdateMouseLook() {
            Vector3 rot(0.0f);
            // jaw
            if (Input::PressedLEFT())
                rot.SetY(360.0f * deltaTime * sensitivity);
            if (Input::PressedRIGHT())
                rot.SetY(-360.0f * deltaTime * sensitivity);
            // pitch
            if (Input::PressedUP())
                rot.SetX(360.0f * deltaTime * sensitivity);
            if (Input::PressedDOWN())
                rot.SetX(-360.0f * deltaTime * sensitivity);

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

            mainCamera.Move(dir * deltaTime);

            if (Input::PressedESC())
                Close();

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
            Material material(SRC_VERTEX, SRC_FRAGMENT);
            // since these are PNGs
            Texture diffuseTex(DIFFUSE_MAP, Texture::TexFormat::RGBA, Texture::PixelFormat::RGBA),
                    specularTex(SPECULAR_MAP, Texture::TexFormat::RGBA, Texture::PixelFormat::RGBA),
                    // since is JPG
                    emissionTex(EMISSION_MAP, Texture::TexFormat::RGB, Texture::PixelFormat::RGB);
            
            // enable ZTest buffering!
            Enable(WndBuffer::Depth);

            cube = new Mesh(rect, sizeof(rect), componentsSize);

            // position attribute
            cube->SetAttribute(0, 3, GL_FLOAT, false, componentsSize, 0);
            // normals
            cube->SetAttribute(1, 3, GL_FLOAT, false, componentsSize, 3 * sizeof(float));
            // uv's attribute
            cube->SetAttribute(2, 2, GL_FLOAT, false, componentsSize, 6 * sizeof(float));
            
            // to bind with current VAO
            cube->Use();
            material.Use();
            // bind diffuse with texture set 0
            glActiveTexture(GL_TEXTURE0); 
            diffuseTex.Use();
            material.SetUniform<int>("diffuseTexture", 0);
            // bind diffuse with texture set 1
            glActiveTexture(GL_TEXTURE1);
            specularTex.Use();
            material.SetUniform<int>("specularTexture", 1);
            // bind diffuse with texture set 1
            glActiveTexture(GL_TEXTURE2);
            emissionTex.Use();
            material.SetUniform<int>("emissionTexture", 2);
            
            // put here to make sure VAO's been binded
            cube->SetTexture(diffuseTex);
            cube->SetTexture(specularTex);
            cube->SetTexture(emissionTex);
            
            cube->SetMaterial(material);
            
            CreateLight(rect, sizeof(rect), componentsSize);
        }

        void OnRenderLight(const Matrix4& view, const Matrix4& proj) {
            Matrix4 model = Matrix4::Indentity();
            Vector3 lightPos(1);
            Vector4 lightPosViewSpace;
            /*
            float t = (float)glfwGetTime();
            lightData.position.z = sin(t) * LIGHT_RADIUS_ROT;
            lightData.position.x = cos(t) * LIGHT_RADIUS_ROT;
            lightData.position.y = 0.0f;
            // color
            lightData.color.r = sin(t * 2.0f);
            lightData.color.g = sin(t * 0.7f);
            lightData.color.b = sin(t * 1.3f);
            */
            // only as white color
            lightData.position = Vector3(1);
            // set white as color
            lightData.color = Vector3(1); 
            lightData.ambient = lightData.color * AMBIENT_FACTOR;

            model = Matrix4::Translate(model, lightPos);
            model = Matrix4::Scale(model, Vector3(0.2f));

            // send pos to cubes shader in view space (reused later in OnRender)
            lightPosViewSpace = 
                Matrix4::Multiply(view * model, Vector4(lightPos, 1.0f));
            lightData.position = lightPosViewSpace.ToVector3();

            // for light's shader            
            lightSource->Render();
            lightSource->SetUniform<Matrix4>("model", model);
            lightSource->SetUniform<Matrix4>("view", view);
            lightSource->SetUniform<Matrix4>("projection", proj);
            lightSource->SetUniform<Vector3>("lightColor", lightData.color);
            lightSource->Draw();
        }

        void OnRender() {
            // transformations
            float angle; 
            Matrix4 model, proj, view = mainCamera.GetViewMatrix();

            proj = Matrix4::Perspective(45.0f, (float)GetAspectRatio(), 0.1f, 100.0f);
            
            OnRenderLight(view, proj);
            
            /*
            //angle = glm::radians(20.0f);
            model = glm::translate(glm::mat4(1.0f), cubeData.transform.Position());
            model = glm::rotate(model, angle, glm::vec3(1, 0.3f, 0.5f));
            */
            angle = (float)glfwGetTime();
            cubeData.transform.SetEulerAngles(Vector3(0.0f));
            model = cubeData.transform.ModelMatrix();
            cube->Render();
            // update uniforms
            cube->SetUniform<Matrix4>("view", view);
            cube->SetUniform<Matrix4>("projection", proj);
            cube->SetUniform<Matrix4>("model", model);
            cube->SetUniform<float>("material.shininess", cubeData.shininess);
            
            cube->SetUniform<Vector3>("light.position", lightData.position);
            cube->SetUniform<Vector3>("light.diffuse", lightData.diffuse);
            cube->SetUniform<Vector3>("light.ambient", lightData.ambient);
            cube->SetUniform<Vector3>("light.specular", lightData.specular);
            cube->SetUniform<Vector3>("light.color", lightData.color);
            
            cube->Draw();

            // update delta time
            float currTime = (float)glfwGetTime();
            deltaTime = currTime - prevTime;
            prevTime = currTime;
        }
    
    private:
        LightData lightData;
        CubeData cubeData;
        Camera mainCamera;
        Mesh* cube;
        Mesh* lightSource;
        float deltaTime, prevTime;

        const float cameraSpeed = 0.1f, sensitivity = 0.1f;
        const float LIGHT_RADIUS_ROT = 2, LIGHT_SPEED = 1, AMBIENT_FACTOR = 0.25f;
        // NOTE: these paths is relative to the 
        //  console's location which is in the "out/..." folder!!!
        const std::string DIFFUSE_MAP = "./../../../../assets/box-container.png",
                          SPECULAR_MAP = "./../../../../assets/box-container-specular.png",
                          EMISSION_MAP = "./../../../../assets/matrix_emission.jpg";
};


int main() {

    std::cout << "Current PATH => " << std::filesystem::current_path() << std::endl;

    LightMaps wnd("LearnOpenGL", 800, 600);

    // init random seed
    srand(time(nullptr));

    wnd.MainLoop();
    return 0;
}