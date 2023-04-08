#pragma once

#include <iostream>
#include <vector>
#include <unordered_map>

#include "shading/Shader.hpp"
#include "shading/Texture.hpp"

#include "material_property_types.hpp"

#include "uniform_buffer.hpp"

class Material {
    public:
        Material();
        Material(const std::string&, const std::string&);
        void Use() const;
        void Render() const;

        void SetShader(const Shader&);
        void SetUniformBlock(const UBO&);

        void SetTexture(const Texture&);
        void SetUniform(const std::string&, const int&);
        void SetUniform(const std::string&, const float&);
        void SetUniform(const std::string&, const Vector3&);
        void SetUniform(const std::string&, const Vector4&);
        void SetUniform(const std::string&, const Matrix4&);
        void SetUniform(const std::string&, const Color&);

        inline bool LinkedToScene() const;

    private:
        Shader shader;
        std::vector<Texture> textures; // each may have different textures...

        bool linkedToScene;
};
