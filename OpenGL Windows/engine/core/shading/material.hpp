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
        Material();
        Material(std::shared_ptr<Shader>);
        // Create with Vertex and Fragment code directly
        Material(const std::string&, const std::string&);

        void Use() const;
        void Render() const;

        void SetShader(std::shared_ptr<Shader>);
        void SetUniformBlock(const UBO&);

        void AddTexture(std::shared_ptr<Texture>, const std::string&);
        void SetUniform(const std::string&, const int&);
        void SetUniform(const std::string&, const float&);
        void SetUniform(const std::string&, const Vector3&);
        void SetUniform(const std::string&, const Vector4&);
        void SetUniform(const std::string&, const Matrix4&);
        void SetUniform(const std::string&, const Color&);

        std::shared_ptr<Shader> InnerShader() const;

    private:
        
        // a material may share the shader with another
        std::shared_ptr<Shader> shader;         
        // each may have different textures...
        std::vector< std::shared_ptr<Texture> > textures;          

        std::vector<uint> linkedUniformBlocks;  // linked uniforms
    };
}

