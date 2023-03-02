#pragma once

#include <iostream>
#include <vector>

#include "shading/Shader.hpp"
#include "shading/Texture.hpp"

class Material {
    public:
        Material();
        Material(const std::string&, const std::string&);
        void Use() const;
        void Render() const;
        void DefineProperties(std::string[], int);
        void AddProperty(const std::string&);
        void SetShader(const Shader&);

        void SetTexture(const Texture&);

        template<class T>
        void SetUniform(const std::string&, const T&);
    private:
        Shader shader;
        std::vector<std::string> properties;
        std::vector<Texture> textures; // each may have different textures...
};

/**
 * Sets inner shader's uniform with its given name and value
*/
template<class T>
void Material::SetUniform(const std::string& propName, const T& value) {
    // shader should overload method with T to avoid errors
    shader.SetUniform(propName, value);
}