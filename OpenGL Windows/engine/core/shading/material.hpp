#pragma once

#include <iostream>
#include <vector>
#include <memory>

#include "shading/Shader.hpp"
#include "shading/Texture.hpp"

#include "material_property_types.hpp"

#include "uniform_buffer.hpp"

namespace CEngine {

    class Material {
    public:

        enum class CullingMode { Front, Back, FrontBack };

        Material();
        Material(std::shared_ptr<Shader>);
        /**
         * Initializes material with given shader src
         * @param vertexSrc Source code of vertex shader
         * @param fragSrc Source code of fragment shader
        */
        Material(const std::string&, const std::string&);

        void Use() const;
        void Render() const;

        void SetShader(std::shared_ptr<Shader>);
        void SetUniformBlock(const UBO&);
        void SetCullingMode(CullingMode);

        void AddTexture(std::shared_ptr<Texture>, const std::string&);
        void SetUniform(const std::string&, const int&);
        void SetUniform(const std::string&, const float&);
        void SetUniform(const std::string&, const Vector3&);
        void SetUniform(const std::string&, const Vector4&);
        void SetUniform(const std::string&, const Matrix4&);
        void SetUniform(const std::string&, const Color&);

        std::shared_ptr<Shader> InnerShader() const;
        CullingMode GetCullingMode() const;

    private:

        void enableInnerFlags() const;
        
        // a material may share the shader with another
        std::shared_ptr<Shader> shader;         
        // each may have different textures...
        std::vector< std::shared_ptr<Texture> > textures; 

        CullingMode cullMode;

        std::vector<uint> linkedUniformBlocks;  // linked uniforms
    };
}

