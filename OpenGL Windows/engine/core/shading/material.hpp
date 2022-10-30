#pragma once

#include <iostream>
#include <vector>

#include "shading/Shader.hpp"

class Material {
    public:
        Material();
        Material(const std::string&, const std::string&);
        void Use() const;
        void DefineProperties(std::string[], int);
        void AddProperty(const std::string&);
        void SetShader(const Shader&);

        template<class T>
        void SetUniform(const std::string&, const T&);
    private:
        Shader shader;
        std::vector<std::string> properties;
};

/**
 * Sets inner shader's uniform with its given name and value
*/
template<class T>
void Material::SetUniform(const std::string& propName, const T& value) {
    // shader should overload method with T to avoid errors
    shader.SetUniform(propName, value);
}